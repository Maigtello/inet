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

#ifndef __INET_SENDWITHFRAGMENTATION_H
#define __INET_SENDWITHFRAGMENTATION_H

#include "inet/protocol/IProtocol.h"
#include "inet/queueing/base/PassivePacketSinkBase.h"

namespace inet {

using namespace inet::queueing;

class INET_API Fragmentation : public PassivePacketSinkBase, public IProtocol
{
  protected:
    b fragmentLength = b(-1);

    cGate *outputGate = nullptr;
    IPassivePacketSink *consumer = nullptr;

  protected:
    virtual void initialize(int stage) override;

  public:
    virtual bool supportsPushPacket(cGate *gate) const override { return true; }
    virtual bool supportsPopPacket(cGate *gate) const override { return false; }

    virtual void pushPacket(Packet *packet, cGate *gate) override;
    virtual void confirm(Packet *packet, bool successful) override;
};

} // namespace inet

#endif // ifndef __INET_SENDWITHFRAGMENTATION_H

