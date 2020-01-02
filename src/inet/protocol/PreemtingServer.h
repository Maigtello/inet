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

#ifndef __PREEMTINGSERVER_H
#define __PREEMTINGSERVER_H

#include "inet/queueing/base/PacketQueueingElementBase.h"
#include "inet/queueing/contract/IActivePacketSink.h"
#include "inet/queueing/contract/IActivePacketSource.h"

namespace inet {

using namespace inet::queueing;

class INET_API PreemtingServer : public PacketQueueingElementBase, public IActivePacketSink, public IActivePacketSource
{
  protected:
    cGate *inputGate = nullptr;
    IPassivePacketSource *provider = nullptr;

    cGate *outputGate = nullptr;
    IPassivePacketSink *consumer = nullptr;

    Packet *packet = nullptr;

  protected:
    virtual void initialize(int stage) override;

    virtual void startSendingPacket();
    virtual void endSendingPacket();

    virtual int getPriority(Packet *packet) const;

  public:
    virtual IPassivePacketSource *getProvider(cGate *gate) override { return provider; }
    virtual IPassivePacketSink *getConsumer(cGate *gate) override { return consumer; }

    virtual bool supportsPushPacket(cGate *gate) const override { return outputGate == gate; }
    virtual bool supportsPopPacket(cGate *gate) const override { return inputGate == gate; }

    virtual void handleCanPushPacket(cGate *gate) override;
    virtual void handleCanPopPacket(cGate *gate) override;

};

} // namespace inet

#endif // ifndef __PREEMTINGSERVER_H

