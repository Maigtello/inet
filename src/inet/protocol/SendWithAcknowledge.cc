//
// Copyright (C) OpenSim Ltd.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see http://www.gnu.org/licenses/.
//

#include "inet/common/IProtocolRegistrationListener.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/protocol/AcknowledgeHeader_m.h"
#include "inet/protocol/SendWithAcknowledge.h"
#include "inet/protocol/SequenceNumberHeader_m.h"

namespace inet {

Define_Module(SendWithAcknowledge);

void SendWithAcknowledge::initialize(int stage)
{
    if (stage == INITSTAGE_LOCAL) {
        acknowledgeTimeout = par("acknowledgeTimeout");
        sequenceNumber = 0;
        registerService(IProtocol::withAcknowledge, gate("in"), gate("in"));
        registerProtocol(IProtocol::withAcknowledge, gate("out"), gate("out"));
        registerService(IProtocol::acknowledge, gate("ackIn"), gate("ackIn"));
    }
}

void SendWithAcknowledge::handleMessage(cMessage *message)
{
    if (message->isPacket()) {
        auto packet = check_and_cast<Packet *>(message);
        auto protocol = packet->getTag<PacketProtocolTag>()->getProtocol();
        if (protocol == &IProtocol::acknowledge) {
            auto header = packet->popAtFront<AcknowledgeHeader>();
            auto sequenceNumber = header->getSequenceNumber();
            auto it = timers.find(sequenceNumber);
            auto timer = it->second;
            timers.erase(it);
            delete packet;
            packet = static_cast<Packet *>(timer->getContextPointer());
            auto input = getConnectedModule<IProtocol>(gate("in"));
            input->confirm(packet, true);
            cancelAndDelete(timer);
        }
        else {
            auto header = makeShared<SequenceNumberHeader>();
            header->setSequenceNumber(sequenceNumber);
            packet->insertAtFront(header);
            packet->addTagIfAbsent<PacketProtocolTag>()->setProtocol(&IProtocol::withAcknowledge);
            send(packet, "out");
            auto timer = new cMessage("AcknowledgeTimer");
            timer->setKind(sequenceNumber);
            timer->setContextPointer(packet);
            timers[sequenceNumber] = timer;
            scheduleAt(simTime() + acknowledgeTimeout, timer);
            sequenceNumber++;
        }
    }
    else {
        auto sequenceNumber = message->getKind();
        timers.erase(timers.find(sequenceNumber));
        auto packet = static_cast<Packet *>(message->getContextPointer());
        auto input = getConnectedModule<IProtocol>(gate("in"));
        input->confirm(packet, false);
        delete message;
    }
}

void SendWithAcknowledge::confirm(Packet *packet, bool successful)
{
    // void
}

} // namespace inet
