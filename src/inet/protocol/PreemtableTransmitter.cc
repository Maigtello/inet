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

#include "inet/protocol/PreemtableTransmitter.h"

namespace inet {

Define_Module(PreemtableTransmitter);

void PreemtableTransmitter::initialize(int stage)
{
    if (stage == INITSTAGE_LOCAL) {
        bitrate = bps(par("bitrate"));
        txEndTimer = new cMessage("TxEndTimer");
    }
}

PreemtableTransmitter::~PreemtableTransmitter()
{
    cancelAndDelete(txEndTimer);
}

void PreemtableTransmitter::handleMessage(cMessage *message)
{
    if (message == txEndTimer)
        endTx();
    else
        throw cRuntimeError("Unknown message");
}


void PreemtableTransmitter::pushPacket(Packet *packet, cGate *gate)
{
    Enter_Method("pushPacket");
    startTx(packet);
}

void PreemtableTransmitter::startTx(Packet *packet)
{
    if (txPacket != nullptr)
        abortTx();
    take(packet);
//    packet->clearTags();
    packet->setDuration(calculateDuration(packet));
    scheduleTxEnd(packet);
    sendPacketStart(packet, gate("out"), packet->getDuration());
    txPacket = packet;
}

void PreemtableTransmitter::endTx()
{
    sendPacketEnd(txPacket, gate("out"), txPacket->getDuration());
    txPacket = nullptr;
}

void PreemtableTransmitter::abortTx()
{
    EV_INFO << "Aborting: packetName = " << txPacket->getName() << std::endl;
    cancelEvent(txEndTimer);
    b transmittedLength = bitrate * s((simTime() - txEndTimer->getSendingTime()).dbl());
//    TODO: txPacket->eraseAtBack(txPacket->getTotalLength() - transmittedLength);
    txPacket->setDuration(calculateDuration(txPacket));
    sendPacketEnd(txPacket, gate("out"), txPacket->getDuration());
    txPacket = nullptr;
}

simtime_t PreemtableTransmitter::calculateDuration(Packet *packet)
{
    return packet->getTotalLength().get() / bitrate.get();
}

void PreemtableTransmitter::scheduleTxEnd(Packet *packet)
{
    scheduleAt(simTime() + packet->getDuration(), txEndTimer);
}

void PreemtableTransmitter::cancelTxEnd()
{
    cancelEvent(txEndTimer);
}

} // namespace inet
