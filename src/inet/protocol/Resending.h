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

#ifndef __INET_SENDWITHRETRIES_H
#define __INET_SENDWITHRETRIES_H

#include "inet/protocol/IProtocol.h"
#include "inet/queueing/contract/IActivePacketSource.h"
#include "inet/queueing/contract/IPassivePacketSink.h"

namespace inet {

class INET_API Resending : public cSimpleModule, public IProtocol, public queueing::IPassivePacketSink
{
  protected:
    int numRetries = 0;
    queueing::IActivePacketSource *producer = nullptr;

    Packet *packet = nullptr;
    int retry = 0;

  protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *message) override;

  public:
    virtual ~Resending() { delete packet; }
    virtual void confirm(Packet *packet, bool successful) override;

    virtual bool canPushSomePacket(cGate *gate = nullptr) const override { return packet == nullptr; }
    virtual bool canPushPacket(Packet *packet, cGate *gate = nullptr) const override { return packet == nullptr; }
    virtual void pushPacket(Packet *packet, cGate *gate = nullptr) override;
};

} // namespace inet

#endif // ifndef __INET_SENDWITHRETRIES_H
