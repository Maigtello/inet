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

#include "inet/protocol/Receiver.h"

namespace inet {

Define_Module(Receiver);

void Receiver::initialize(int stage)
{
    if (stage == INITSTAGE_LOCAL) {
        bitrate = bps(par("bitrate"));
    }
}

Receiver::~Receiver()
{
    delete rxPacket;
}

void Receiver::handleMessage(cMessage *message)
{
    if (message->arrivedOn("in"))
        receiveFromMedium(message);
    else
        throw cRuntimeError("Unknown message");
}


void Receiver::pushPacket(Packet *packet, cGate *gate)
{
}

void Receiver::sendToUpperLayer(Packet *packet)
{
    send(packet, gate("out"));
}

void Receiver::receivePacketStart(cPacket *packet)
{
    startRx(check_and_cast<Packet *>(packet));
}

void Receiver::receivePacketEnd(cPacket *packet)
{
    endRx(check_and_cast<Packet *>(packet));
}

void Receiver::startRx(Packet *packet)
{
    take(packet);
    rxPacket = packet;
}

void Receiver::endRx(Packet *packet)
{
    sendToUpperLayer(packet);
    rxPacket = nullptr;
}

} // namespace inet
