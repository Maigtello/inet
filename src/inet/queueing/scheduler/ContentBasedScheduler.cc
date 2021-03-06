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

#include "inet/queueing/scheduler/ContentBasedScheduler.h"

namespace inet {
namespace queueing {

Define_Module(ContentBasedScheduler);

ContentBasedScheduler::~ContentBasedScheduler()
{
    for (auto filter : filters)
        delete filter;
}

void ContentBasedScheduler::initialize(int stage)
{
    PacketSchedulerBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        defaultGateIndex = par("defaultGateIndex");
        cStringTokenizer packetFilterTokenizer(par("packetFilters"), ";");
        cStringTokenizer packetDataFilterTokenizer(par("packetDataFilters"), ";");
        while (packetFilterTokenizer.hasMoreTokens() && packetDataFilterTokenizer.hasMoreTokens()) {
            auto filter = new PacketFilter();
            filter->setPattern(packetFilterTokenizer.nextToken(), packetDataFilterTokenizer.nextToken());
            filters.push_back(filter);
        }
    }
}

int ContentBasedScheduler::schedulePacket()
{
    for (auto filter : filters) {
        for (int i = 0; i < (int)inputGates.size(); i++) {
            auto packet = providers[i]->canPopPacket(inputGates[i]);
            if (packet != nullptr && filter->matches(packet))
                return i;
        }
    }
    return defaultGateIndex;
}

} // namespace queueing
} // namespace inet

