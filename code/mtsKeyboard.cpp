/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id$

  Author(s):	Gorkem Sevinc, Anton Deguet
  Created on:   2009-09-17

  (C) Copyright 2009-2011 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/


#include <sawKeyboard/mtsKeyboard.h>
#include <cisstCommon/cmnGetChar.h>
#include <cisstMultiTask/mtsInterfaceRequired.h>
#include <cisstMultiTask/mtsInterfaceProvided.h>
#include <cisstParameterTypes/prmEventButton.h>


CMN_IMPLEMENT_SERVICES(mtsKeyboard);

mtsKeyboard::mtsKeyboard(void):
    mtsTaskContinuous("keyboard")
{
    DoneMember = false;
}


void mtsKeyboard::AddKeyButtonEvent(char key, const std::string & interfaceName, bool toggle)
{
    // create new key data and add to list
    KeyData * keyData = new KeyData;
    KeyboardDataMap.insert(std::make_pair(key, keyData));

    // set key action
    keyData->Type = BUTTON_EVENT;
    keyData->Toggle = toggle;
    keyData->State = false;

    // add interface
    mtsInterfaceProvided * providedInterface = this->GetInterfaceProvided(interfaceName);
    if (!providedInterface) {
        providedInterface = this->AddInterfaceProvided(interfaceName);
    }
    keyData->WriteTrigger.Bind(providedInterface->AddEventWrite("Button",
                                                                prmEventButton()));
}


void mtsKeyboard::AddKeyVoidEvent(char key, const std::string & interfaceName, const std::string & eventName)
{
    // create new key data and add to list
    KeyData * keyData = new KeyData;
    KeyboardDataMap.insert(std::make_pair(key, keyData));  

    // set key action
    keyData->Type = VOID_EVENT;
    keyData->State = false;

    // add interface
    mtsInterfaceProvided * providedInterface = this->GetInterfaceProvided(interfaceName);
    if (!providedInterface) {
        providedInterface = this->AddInterfaceProvided(interfaceName);
    }
    keyData->VoidTrigger.Bind(providedInterface->AddEventVoid(eventName));
}


void mtsKeyboard::AddKeyVoidFunction(char key, const std::string & interfaceName, const std::string & commandName)
{
    // create new key data and add to list
    KeyData * keyData = new KeyData;
    KeyboardDataMap.insert(std::make_pair(key, keyData));

    // set key action
    keyData->Type = VOID_FUNCTION;
    keyData->State = false;

    // add interface
    mtsInterfaceRequired * requiredInterface = this->GetInterfaceRequired(interfaceName);
    if (!requiredInterface) {
        requiredInterface = this->AddInterfaceRequired(interfaceName);
    }
    requiredInterface->AddFunction(commandName, keyData->VoidTrigger);
}


void mtsKeyboard::AddKeyWriteFunction(char key, const std::string & interfaceName, const std::string & commandName, bool initialState)
{
    // create new key data and add to list
    KeyData * keyData = new KeyData;
    KeyboardDataMap.insert(std::make_pair(key, keyData));

    // set key action
    keyData->Type = WRITE_FUNCTION;
    keyData->State = initialState;

    // add interface
    mtsInterfaceRequired * requiredInterface = this->GetInterfaceRequired(interfaceName);
    if (!requiredInterface) {
        requiredInterface = this->AddInterfaceRequired(interfaceName);
    }
    requiredInterface->AddFunction(commandName, keyData->WriteTrigger);
}


void mtsKeyboard::SetQuitKey(char quitKey)
{
    QuitKey = quitKey;
}


void mtsKeyboard::Run(void) 
{
    KeyboardInput = cmnGetChar();
    if (KeyboardInput == QuitKey) {
        DoneMember = true;
    }

    if (!this->Done()) {
        prmEventButton event;
        KeyData * keyData;
        KeyDataType::iterator iterator;
        const KeyDataType::iterator end = this->KeyboardDataMap.end();
        for (iterator = this->KeyboardDataMap.begin();
             iterator != end;
             iterator++)
            {
                if (iterator->first == KeyboardInput) {
                    keyData = iterator->second;
                    switch (keyData->Type) {
                    case BUTTON_EVENT:
                        if (keyData->Toggle == true) {
                            if (keyData->State == true) {
                                event.SetType(prmEventButton::RELEASED);
                                keyData->State = false;
                            } else {
                                event.SetType(prmEventButton::PRESSED);
                                keyData->State = true;
                            }
                        } else {
                            event.SetType(prmEventButton::RELEASED);
                            keyData->State = true;
                        }
                        keyData->WriteTrigger(event);
                        CMN_LOG_CLASS_RUN_DEBUG << "Run " << KeyboardInput << " sending button event " << event << std::endl;
                        break;
                    case VOID_EVENT:
                        keyData->VoidTrigger();
                        CMN_LOG_CLASS_RUN_DEBUG << "Run " << KeyboardInput << " sending void event " << std::endl;
                        break;
                    case VOID_FUNCTION:
                        keyData->VoidTrigger();
                        CMN_LOG_CLASS_RUN_DEBUG << "Run " << KeyboardInput << " calling void function " << std::endl;
                        break;
                    case WRITE_FUNCTION:
                        mtsBool value = keyData->State;
                        keyData->WriteTrigger(value);
                        keyData->State = !(keyData->State);
                        CMN_LOG_CLASS_RUN_DEBUG << "Run " << KeyboardInput << " calling write command " << value << std::endl;
                        break;
                    }
                }
            }
    }
}
