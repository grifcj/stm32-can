*** Settings ***
Suite Setup                   Setup
Suite Teardown                Teardown
Test Setup                    My Test Setup
Test Teardown                 Test Teardown
Resource                      ${RENODEKEYWORDS}

*** Keywords ***
My Test Setup
    Reset Emulation
    Execute Command         path add @${CURDIR}

Send CAN Message
    Execute Command         mach set "sender"
    Execute Command         runMacro $sendCANMessage

Received CAN Message
    [Arguments]             ${tester}
    Wait For Line On Uart   Received CAN message  testerId=${tester}  timeout=0.5
    Wait For Line On Uart   Received data matches transmitted data  testerId=${tester}  timeout=0.5

*** Test Cases ***
Polling loopback can send and receive message
    Execute Command         set bin @build/loopback
    Execute Command         include @renode-loopback.resc

    ${tester} =  Create Terminal Tester  sysbus.uart1

    Start Emulation

    Send CAN Message
    Received CAN Message  ${tester}

Interrupt loopback can send and receive message
    Execute Command         set bin @build/loopback_interrupt
    Execute Command         include @renode-loopback.resc

    ${tester} =  Create Terminal Tester  sysbus.uart1

    Start Emulation

    Send CAN Message
    Received CAN Message  ${tester}

Two controllers can communicate across CANHub
    Execute Command         set bin @build/interrupt
    Execute Command         include @renode-canhub.resc

    ${tester1} =  Create Terminal Tester  sysbus.uart1  machine=receiver1
    ${tester2} =  Create Terminal Tester  sysbus.uart1  machine=receiver2

    Start Emulation

    Send CAN Message

    Received CAN Message  ${tester1}
    Received CAN Message  ${tester2}
