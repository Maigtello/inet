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

#ifndef __INET_TRANSMITTER_H
#define __INET_TRANSMITTER_H

#include "inet/common/IProtocolRegistrationListener.h"
#include "inet/protocol/IProtocol.h"

namespace inet {

class INET_API Transmitter : public cSimpleModule, public IProtocol
{
  protected:
    bps datarate = bps(NaN);
    Packet *packet = nullptr;
    cMessage *endTimer = nullptr;

  protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *message) override;

  public:
    virtual ~Transmitter() { cancelAndDelete(endTimer); }
    virtual void confirm(Packet *packet, bool successful) override;
};

} // namespace inet

#endif // ifndef __INET_TRANSMITTER_H
