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

package inet.examples.test;

import inet.protocol.Defragmentation;
import inet.protocol.DynamicClassifier;
import inet.protocol.Fragmentation;
import inet.protocol.PreemtableTransmitter;
import inet.protocol.PreemtingServer;
import inet.protocol.Receiver;
import inet.protocol.SequenceNumbering;
import inet.queueing.common.PacketMultiplexer;
import inet.queueing.marker.PacketTagger;
import inet.queueing.queue.PacketQueue;
import inet.queueing.sink.PassivePacketSink;
import inet.queueing.source.ActivePacketSource;

module SenderHost
{
    parameters:
        @networkNode;
        @display("i=device/pc");
    gates:
        output g;
    submodules:
        source1: ActivePacketSource {
            @display("p=150,100");
        }
        tagger1: PacketTagger {
            @display("p=150,200");
        }
        source2: ActivePacketSource {
            @display("p=250,100");
        }
        tagger2: PacketTagger {
            @display("p=250,200");
        }
        multiplexer: PacketMultiplexer {
            @display("p=200,300");
        }
        sequenceNumbering: SequenceNumbering {
            @display("p=200,400");
        }
        queue: PacketQueue {
            @display("p=200,500");
        }
        server: PreemtingServer {
            @display("p=200,600");
        }
        fragmentation: Fragmentation {
            @display("p=200,700");
        }
        transmitter: PreemtableTransmitter {
            @display("p=200,800");
        }
    connections:
        source1.out --> tagger1.in;
        source2.out --> tagger2.in;
        tagger1.out --> multiplexer.in++;
        tagger2.out --> multiplexer.in++;
        multiplexer.out --> sequenceNumbering.in;
        sequenceNumbering.out --> queue.in;
        queue.out --> server.in;
        server.out --> fragmentation.in;
        fragmentation.out --> transmitter.in;
        transmitter.out --> g;
}

module DynamicDefragmentation
{
    parameters:
        int numDefragmentation = default(0);
        @display("i=block/routing");
    gates:
        input in;
        output out;
    submodules:
        multiplexer: PacketMultiplexer {
            @display("p=150,100");
        }
        defragmentation[numDefragmentation]: Defragmentation {
            @display("p=150,200,row,100");
        }
        classifier: DynamicClassifier {
            @display("p=150,300");
        }
    connections:
        in --> classifier.in;
        for i=0..numDefragmentation-1 {
            classifier.out++ --> multiplexer.in++;
        }
        multiplexer.out --> out;
}

module ReceiverHost
{
    parameters:
        @networkNode;
        @display("i=device/pc");
    gates:
        input g;
    submodules:
        sink: PassivePacketSink {
            @display("p=150,100");
        }
        defragmentation: DynamicDefragmentation {
            @display("p=150,200");
        }
        receiver: Receiver {
            @display("p=150,300");
        }
    connections:
        g --> receiver.in;
        receiver.out --> defragmentation.in;
        defragmentation.out --> sink.in;
}

network SimpleNetwork
{
    submodules:
        sender: SenderHost {
            @display("p=100,100");
        }
        receiver: ReceiverHost {
            @display("p=200,100");
        }
    connections:
        sender.g --> {  delay = 1us; } --> receiver.g;
}
