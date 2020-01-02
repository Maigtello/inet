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

#ifndef __PREEMTABLETRANSMITTER_H
#define __PREEMTABLETRANSMITTER_H

#include "inet/queueing/contract/IPacketQueueingElement.h"
#include "inet/queueing/contract/IPassivePacketSink.h"

namespace inet {

using namespace inet::units::values;
using namespace inet::queueing;

class INET_API PreemtableTransmitter : public cPhyModule, public IPassivePacketSink, public IPacketQueueingElement
{
  protected:
    bps bitrate = bps(NaN);

    cMessage *txEndTimer = nullptr;

    Packet *txPacket = nullptr;

  protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *message) override;

    virtual void startTx(Packet *packet);
    virtual void endTx();
    virtual void abortTx();

    virtual simtime_t calculateDuration(Packet *packet);

    virtual void scheduleTxEnd(Packet *packet);
    virtual void cancelTxEnd();

  public:
    virtual ~PreemtableTransmitter();

    virtual bool supportsPushPacket(cGate *gate) const override { return gate == this->gate("in"); }
    virtual bool supportsPopPacket(cGate *gate) const override { return false; }

    virtual bool canPushSomePacket(cGate *gate = nullptr) const override { return txPacket == nullptr; };
    virtual bool canPushPacket(Packet *packet, cGate *gate = nullptr) const override { return txPacket == nullptr; };
    virtual void pushPacket(Packet *packet, cGate *gate = nullptr) override;
};

} // namespace inet

#endif // ifndef __PREEMTABLETRANSMITTER_H

