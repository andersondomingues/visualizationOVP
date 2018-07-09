#
# Copyright (c) 2005-2017 Imperas Software Ltd., www.imperas.com
#
# The contents of this file are provided under the Software License
# Agreement that you accepted before downloading this file.
#
# This source forms part of the Software and can be used for educational,
# training, and demonstration purposes but cannot be used for derivative
# works except in cases where the derivative works require OVP technology
# to run.
#
# For open source models released under licenses that you can use for
# derivative works, please visit www.OVPworld.org or www.imperas.com
# for the location of the open source models.
#

ihwnew -name resetControl  \
        -vendor example.ovpworld.org  \
        -library platform  \
        -version 1.0 \
        -purpose baremetal -releasestatus ovp

iadddocumentation  \
        -name Description  \
        -text "Design instancing two processors
               using http visualization to control the reset of the processors."

iadddocumentation  \
        -name Licensing  \
        -text "Open Source Apache 2.0"

iadddocumentation  \
        -name Limitations  \
        -text "None"

iadddocumentation  \
        -name Reference  \
        -text "Simple example hardware module design."


# processor memory map
# 0x00000000-0xdfffffff  : local memory
# 0xe0000000-0xefffffff  : shared region
# 0xf0000000-0xffffffff  : local memory


#------------------------------------------------------------------------

#                  ______________[shared]_______________
#                 |                                    |
#     [mem1a]--[bus1]--[mem1b]              [mem2a]--[bus2]--[mem2b]
#                ||                                    ||
#     [rst1]--[cpu1]                                 [cpu2]--[rst2]
#
#
# ------------------------------------------------------------ first sub-system
ihwaddbus -instancename bus1 -addresswidth 32

# reset connection
ihwaddnet -instancename reset1

ihwaddprocessor -instancename cpu1 \
                -vendor ovpworld.org -library processor -type or1k -version 1.0 \
                -semihostname or1kNewlib \
                -variant generic

ihwconnect -bus bus1 -instancename cpu1 -busmasterport INSTRUCTION
ihwconnect -bus bus1 -instancename cpu1 -busmasterport DATA

# connect reset1 to cpu1
ihwconnect -instancename cpu1 -netport reset -net reset1

ihwaddmemory -instancename mem1_1 -type ram
ihwconnect   -instancename mem1_1 -bus bus1 -busslaveport sp1 -loaddress 0x00000000 -hiaddress 0xdfffffff
ihwaddmemory -instancename mem1_2 -type ram
ihwconnect   -instancename mem1_2 -bus bus1 -busslaveport sp1 -loaddress 0xf0000000 -hiaddress 0xffffffff

# ----------------------------------------------------------- second sub-system
ihwaddbus -instancename bus2 -addresswidth 32

# reset connection
ihwaddnet -instancename reset2

ihwaddprocessor -instancename cpu2 \
                -vendor ovpworld.org -library processor -type or1k -version 1.0 \
                -semihostname or1kNewlib \
                -variant generic

ihwconnect -bus bus2 -instancename cpu2 -busmasterport INSTRUCTION
ihwconnect -bus bus2 -instancename cpu2 -busmasterport DATA

# connect reset2 to cpu2
ihwconnect -instancename cpu2 -netport reset -net reset2

ihwaddmemory -instancename mem2_1 -type ram
ihwconnect   -instancename mem2_1 -bus bus2 -busslaveport sp1 -loaddress 0x00000000 -hiaddress 0xdfffffff
ihwaddmemory -instancename mem2_2 -type ram
ihwconnect   -instancename mem2_2 -bus bus2 -busslaveport sp1 -loaddress 0xf0000000 -hiaddress 0xffffffff

# ----------------------------------------------------------- shared mem
#ihwaddbus -instancename busS -addresswidth 32
ihwaddmemory -instancename memShared -type ram
ihwconnect   -instancename memShared -bus bus1 -busslaveport shared1 -loaddress 0xe0000000 -hiaddress 0xefffffff
ihwconnect   -instancename memShared -bus bus2 -busslaveport shared2 -loaddress 0xe0000000 -hiaddress 0xefffffff

#------------------------------------------------------------ interruption
# connect interruptCpu1 to cpu1 intr0
ihwaddnet -instancename interruptCpu1
ihwconnect -instancename cpu1 -netport intr0 -net interruptCpu1

ihwaddnet -instancename interruptCpu2
ihwconnect -instancename cpu2 -netport intr0 -net interruptCpu2