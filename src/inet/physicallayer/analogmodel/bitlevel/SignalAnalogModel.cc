//
// Copyright (C) 2013 OpenSim Ltd.
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
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#include "inet/physicallayer/analogmodel/bitlevel/SignalAnalogModel.h"

namespace inet {

namespace physicallayer {

SignalAnalogModel::SignalAnalogModel(const simtime_t duration) :
    duration(duration)
{
}

std::ostream& SignalAnalogModel::printToStream(std::ostream& stream, int level) const
{
    if (level <= PRINT_LEVEL_TRACE)
        stream << ", duration = " << duration;
    return stream;
}

NarrowbandSignalAnalogModel::NarrowbandSignalAnalogModel(const simtime_t duration, Hz centerFrequency, Hz bandwidth) :
    SignalAnalogModel(duration),
    centerFrequency(centerFrequency),
    bandwidth(bandwidth)
{
}

std::ostream& NarrowbandSignalAnalogModel::printToStream(std::ostream& stream, int level) const
{
    if (level <= PRINT_LEVEL_DEBUG)
       stream << ", centerFrequency = " << centerFrequency;
    if (level <= PRINT_LEVEL_TRACE)
       stream << ", bandwidth = " << bandwidth;
    return stream;
}

} // namespace physicallayer

} // namespace inet

