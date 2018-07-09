/*
 *
 * Copyright (c) 2005-2017 Imperas Software Ltd., www.imperas.com
 *
 * The contents of this file are provided under the Software License
 * Agreement that you accepted before downloading this file.
 *
 * This source forms part of the Software and can be used for educational,
 * training, and demonstration purposes but cannot be used for derivative
 * works except in cases where the derivative works require OVP technology
 * to run.
 *
 * For open source models released under licenses that you can use for
 * derivative works, please visit www.OVPworld.org or www.imperas.com
 * for the location of the open source models.
 *
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "op/op.h"
#include "ocl/oclhttp.h"

#define ROOT_NAME "top"
#define MODULE_INSTANCE "u1"
#define CPU1_INSTANCE "cpu1"
#define CPU2_INSTANCE "cpu2"

#define PREFIX "HARNESS"

typedef struct myDataS {
    optModuleP      mi;
    optNetP         reset1;
    optNetP         reset2;
    optNetP			interrupt1;
    optNetP			interrupt2;
    optProcessorP   cpu1;
    optProcessorP   cpu2;
} myData, *myDataP;

static OCL_HTTP_GET_FN(get) {
    myDataP my = userData;
    oclHTTPElementOpen(ch, "ovpelement");
    oclHTTPKeyPrintf(ch, "moduleInstName0", opObjectName(my->mi));
    oclHTTPKeyPrintf(ch, "moduleSimTime0","%g", (double)opModuleCurrentTime(opObjectRootModule(my->reset1)) );
    oclHTTPKeyPrintf(ch, "reset1", "%d", opNetValue(my->reset1));
    oclHTTPKeyPrintf(ch, "cpu1icount", FMT_64d, opProcessorICount(my->cpu1));
    oclHTTPKeyPrintf(ch, "cpu2icount", FMT_64d, opProcessorICount(my->cpu2));
    oclHTTPKeyPrintf(ch, "reset2", "%d", opNetValue(my->reset2));
    oclHTTPKeyPrintf(ch, "power0", "0");  
    oclHTTPKeyPrintf(ch, "cpu1int", "%d", opNetValue(my->interrupt1));
    oclHTTPKeyPrintf(ch, "cpu2int", "%d", opNetValue(my->interrupt2));

    oclHTTPKeyPrintf(ch, "cpuName1", "%s", opObjectName(my->cpu1));
    oclHTTPKeyPrintf(ch, "cpuName2", "%s", opObjectName(my->cpu2));
        
    oclHTTPElementClose(ch, "ovpelement");
    oclHTTPSend(ch);
}

static OCL_HTTP_POST_FN(post) {
    myDataP my = userData;
    if (strstr(body, "power0=clicked")) {
        opMessage("I", PREFIX "_SW", "Power Switch pushed - terminating simulation.");
        opModuleFinish(my->mi, 0);
    } else if (strstr(body, "reset1=clicked")) {
        opMessage("I", PREFIX "_SW", "RESET 1 pushed.");
        opNetWrite(my->reset1, (opNetValue(my->reset1) == 1)?0:1);
    } else if (strstr(body, "reset2=clicked")) {
        opMessage("I", PREFIX "_SW", "RESET 2 pushed");
        opNetWrite(my->reset2, (opNetValue(my->reset2) == 1)?0:1);
    }
}

static void exitWithErrors(void) {
    opSessionExit(1);
}

static optParamSpec paramSpecs[] = {
    OP_HTTP_VIS_PORT_FORMALS,
    { 0 }
};

static OP_PARAM_SPEC_FN(harnessParamIterator) {
    prev = prev ? prev + 1 : paramSpecs;
    return prev->name ? prev : 0;
}

optModuleAttr modelAttrs = {
    .versionString        = OP_VERSION,
    .type                 = OP_MODULE,
    .name                 = "harness",
    .paramSpecsCB         = harnessParamIterator,
};


int main(int argc, const char *argv[]) {
    
    myData mys;
    myDataP my = &mys;

    Uns32 httpvisportnum = 8000;   // default port number
    
    opSessionInit(OP_VERSION);

    optCmdParserP parser = opCmdParserNew(argv[0], OP_AC_ALL);
 
    opCmdParserAdd (parser, "httpvisportnum",    0, 0, "harness",
                    OP_FT_UNS32VAL, &httpvisportnum,
                    "HTTP visualization port number", OP_AC_ALL, 0, 0);
    
    Bool ok = opCmdParseArgs(parser, argc, argv);
    if(!ok) {
        exitWithErrors();
    }
    
    optModuleP mr = opRootModuleNew(&modelAttrs, ROOT_NAME, 0);

    optModuleP mi = opModuleNew(
        mr,                 // parent module
        "module",            // modelfile
        "u1",    // name
        0,
        0
    );

    // construction complete       
    octHTTPMethods m = { .get=get, .post=post, .userData=my };

    opModuleHTTPOpen(mr, &m, httpvisportnum, "harness/httpvis");
    
    opRootModulePreSimulate(mr);

    // save module
    my->mi = mi;
    
    // save reset nets
    my->interrupt1 = opObjectByName(mi, "interruptCpu1", OP_NET_EN).Net;
    my->interrupt2 = opObjectByName(mi, "interruptCpu2", OP_NET_EN).Net;
    my->reset1 = opObjectByName(mi, "reset1", OP_NET_EN).Net;
    my->reset2 = opObjectByName(mi, "reset2", OP_NET_EN).Net;
    my->cpu1 = opObjectByName(mi, MODULE_INSTANCE "/" CPU1_INSTANCE, OP_PROCESSOR_EN).Processor;
    my->cpu2 = opObjectByName(mi, MODULE_INSTANCE "/" CPU2_INSTANCE, OP_PROCESSOR_EN).Processor;

    // Release processors
    opMessage("I", "HARNESS", "Release processor cpu1");
    opNetWrite(my->reset1, 0);
    opMessage("I", "HARNESS", "Release processor cpu2");
    opNetWrite(my->reset2, 0);

    opMessage("I", "HARNESS", "Simulate to End");
    opRootModuleSimulate(mr);

    opMessage("I", "HARNESS", "Simulation finished\n");

    // get the handle for the processor in the module
    optProcessorP processor = 0;
    while ((processor = opProcessorNext(mi, processor))) {
        // print number of instructions executed at end of simulation and reason simulation stopped
        opMessage("I", "HARNESS", "%s executed "FMT_64u" instructions, StopReason '%s'\n",
                    opObjectName(processor),
                    opProcessorICount(processor),
                    opStopReasonString(opProcessorStopReason(processor))
        );
    }

    opSessionTerminate();
    return 0;
}

