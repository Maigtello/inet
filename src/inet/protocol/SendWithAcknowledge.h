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

#ifndef __INET_SENDWITHACKNOWLEDGE_H
#define __INET_SENDWITHACKNOWLEDGE_H

#include "inet/protocol/IProtocol.h"

namespace inet {

class INET_API SendWithAcknowledge : public cSimpleModule, public IProtocol
{
  protected:
    simtime_t acknowledgeTimeout = -1;

    int sequenceNumber = -1;
    std::map<int, cMessage *> timers;

  protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *message) override;

  public:
    virtual void confirm(Packet *packet, bool successful) override;
};

} // namespace inet

#endif // ifndef __INET_SENDWITHACKNOWLEDGE_H
