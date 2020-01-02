//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include "inet/common/ModuleAccess.h"
#include "inet/linklayer/common/UserPriorityTag_m.h"
#include "inet/protocol/PreemtingServer.h"

namespace inet {

Define_Module(PreemtingServer);

void PreemtingServer::initialize(int stage)
{
    PacketQueueingElementBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        inputGate = gate("in");
        provider = getConnectedModule<IPassivePacketSource>(inputGate);
        outputGate = gate("out");
        consumer = getConnectedModule<IPassivePacketSink>(outputGate);
    }
    else if (stage == INITSTAGE_QUEUEING) {
        checkPopPacketSupport(inputGate);
        checkPushPacketSupport(outputGate);
    }
}

void PreemtingServer::startSendingPacket()
{
    packet = provider->popPacket(inputGate->getPathStartGate());
    take(packet);
    packet->setArrival(getId(), inputGate->getId(), simTime());
    EV_INFO << "Sending packet " << packet->getName() << " started." << endl;
    pushOrSendPacket(packet, outputGate, consumer);
}

void PreemtingServer::endSendingPacket()
{
    EV_INFO << "Sending packet " << packet->getName() << " ended.\n";
    packet = nullptr;
}

int PreemtingServer::getPriority(Packet *packet) const
{
    return packet->getTag<UserPriorityReq>()->getUserPriority();
}

void PreemtingServer::handleCanPushPacket(cGate *gate)
{
    Enter_Method("handleCanPushPacket");
    if (packet != nullptr)
        endSendingPacket();
    if (provider->canPopSomePacket(inputGate->getPathStartGate()))
        startSendingPacket();
}

void PreemtingServer::handleCanPopPacket(cGate *gate)
{
    Enter_Method("handleCanPopPacket");
    if (consumer->canPushSomePacket(outputGate->getPathEndGate()))
        startSendingPacket();
    else {
        auto nextPacket = provider->canPopPacket(inputGate->getPathStartGate());
        if (getPriority(nextPacket) > getPriority(packet)) {
            // TODO: truncate current packet and push back remainder into queue
//            b offset = b(0);
//            Packet *remainingPacket = new Packet(packet->getName(), packet->peekAt());
            startSendingPacket();
        }
    }
}

} // namespace inet
