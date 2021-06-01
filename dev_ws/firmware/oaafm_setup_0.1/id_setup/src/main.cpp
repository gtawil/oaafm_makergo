/*******************************************************************************
* Copyright 2016 ROBOTIS CO., LTD.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/
#include "Arduino.h"
#include <Dynamixel2Arduino.h>


/* Replace with the Model number :
        -XC430
        -XM540
        -XM430
        -_2XL430
        -XL320

**/
#define _2XL430


#if defined(XC430)
const uint8_t DEFAULT_DXL_ID = 1;
const uint8_t NEW_ID = 1;
const uint32_t DEFAULT_BAUDRATE = 57600;
const uint32_t NEW_BAUDRATE = 57600;
#elif defined(XM540)
const uint8_t DEFAULT_DXL_ID = 1;
const uint8_t NEW_ID = 2;
const uint32_t DEFAULT_BAUDRATE = 57600;
const uint32_t NEW_BAUDRATE = 57600;
#elif defined(XM430)
const uint8_t DEFAULT_DXL_ID = 1;
const uint8_t NEW_ID = 3;
const uint32_t DEFAULT_BAUDRATE = 57600;
const uint32_t NEW_BAUDRATE = 57600;
#elif defined(_2XL430)
const uint8_t DEFAULT_DXL_A_ID = 1;
const uint8_t NEW_A_ID = 4;
const uint8_t DEFAULT_DXL_B_ID = 2;
const uint8_t NEW_B_ID = 5;
const uint32_t DEFAULT_BAUDRATE = 57600;
const uint32_t NEW_BAUDRATE = 57600;
#elif defined(XL320)
const uint8_t DEFAULT_DXL_ID = 1;
const uint8_t NEW_ID = 6;
const uint32_t DEFAULT_BAUDRATE = 1000000;
const uint32_t NEW_BAUDRATE = 57600;
#endif

#define TIME_BEFORE_MESSAGE 500
#define TIME_AFTER_MESSAGE 500


#define DXL_SERIAL Serial
#define DEBUG_SERIAL Serial
const uint8_t DXL_DIR_PIN = 2; // DYNAMIXEL Shield DIR PIN

const float DXL_PROTOCOL_VERSION = 2.0;

Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);

//This namespace is required to use Control table item names
using namespace ControlTableItem;

void setup()
{

    /*
        Depend on the model
    **/

    /****************************************************************
        For Model : 2 XL 430
    ****************************************************************/

    delay(5000);
    #ifdef _2XL430

    uint8_t present_a_id = DEFAULT_DXL_A_ID;
    uint8_t new_a_id = NEW_A_ID;
    uint8_t present_b_id = DEFAULT_DXL_B_ID;
    uint8_t new_b_id = NEW_B_ID;

    // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
    dxl.begin(DEFAULT_BAUDRATE);
    // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
    dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);


    delay(TIME_BEFORE_MESSAGE);
    DEBUG_SERIAL.println();
    DEBUG_SERIAL.print("ID : "); DEBUG_SERIAL.println(present_a_id);
    DEBUG_SERIAL.println();
    delay(TIME_AFTER_MESSAGE);

    
    if (dxl.ping(present_a_id) == true)
    {
        delay(TIME_BEFORE_MESSAGE);
        DEBUG_SERIAL.println();
        DEBUG_SERIAL.println("ping succeeded!");
        DEBUG_SERIAL.println();
        delay(TIME_AFTER_MESSAGE);

        uint8_t model = dxl.getModelNumber(present_a_id);

        delay(TIME_BEFORE_MESSAGE);
        DEBUG_SERIAL.println();
        DEBUG_SERIAL.print("Model Number: "); DEBUG_SERIAL.println(model);
        DEBUG_SERIAL.println();
        delay(TIME_AFTER_MESSAGE);

        // Turn off torque when configuring items in EEPROM area
        dxl.torqueOff(present_a_id);

        if (dxl.setID(present_a_id, new_a_id) == true)
        {
            uint8_t old_a_id = present_a_id;
            present_a_id = new_a_id;

            delay(TIME_BEFORE_MESSAGE);
            DEBUG_SERIAL.println();
            DEBUG_SERIAL.print("ID has been successfully changed to "); DEBUG_SERIAL.println(new_a_id);
            DEBUG_SERIAL.println();
            delay(TIME_AFTER_MESSAGE);

            if (dxl.ping(present_a_id) == true)
            {
                delay(TIME_BEFORE_MESSAGE);
                DEBUG_SERIAL.println();
                DEBUG_SERIAL.print("ID : "); DEBUG_SERIAL.print(present_a_id); DEBUG_SERIAL.println(" :  ping succeeded!");
                DEBUG_SERIAL.println();
                delay(TIME_AFTER_MESSAGE);
            }
            else
            {
                delay(TIME_BEFORE_MESSAGE);
                DEBUG_SERIAL.println();
                DEBUG_SERIAL.print("Failed to detect ID : "); DEBUG_SERIAL.println(present_a_id);
                DEBUG_SERIAL.print("Try to detect ID : "); DEBUG_SERIAL.println(old_a_id);
                DEBUG_SERIAL.println();
                delay(TIME_AFTER_MESSAGE);

                if (dxl.ping(old_a_id) == true)
                {
                    delay(TIME_BEFORE_MESSAGE);
                    DEBUG_SERIAL.println();
                    DEBUG_SERIAL.print("ID : "); DEBUG_SERIAL.print(old_a_id); DEBUG_SERIAL.println(" :  ping succeeded!");
                    DEBUG_SERIAL.println();
                    delay(TIME_AFTER_MESSAGE);
                }
                else
                {
                    delay(TIME_BEFORE_MESSAGE);
                    DEBUG_SERIAL.println();
                    DEBUG_SERIAL.print("Failed to detect ID : "); DEBUG_SERIAL.println(old_a_id);
                    DEBUG_SERIAL.println();
                    delay(TIME_AFTER_MESSAGE);
                }
            }
        }
        else
        {
            delay(TIME_BEFORE_MESSAGE);
            DEBUG_SERIAL.println();
            DEBUG_SERIAL.print("Failed to change ID to "); DEBUG_SERIAL.println(new_a_id);
            DEBUG_SERIAL.println();
            delay(TIME_AFTER_MESSAGE);
        }
    }
    else
    {
        delay(TIME_BEFORE_MESSAGE);
        DEBUG_SERIAL.println("ping failed!");
        delay(TIME_AFTER_MESSAGE);
    }

    delay(TIME_BEFORE_MESSAGE);
    DEBUG_SERIAL.println();
    DEBUG_SERIAL.print("ID : "); DEBUG_SERIAL.println(present_b_id);
    DEBUG_SERIAL.println();
    delay(TIME_AFTER_MESSAGE);

    
    if (dxl.ping(present_b_id) == true)
    {
        delay(TIME_BEFORE_MESSAGE);
        DEBUG_SERIAL.println();
        DEBUG_SERIAL.println("ping succeeded!");
        DEBUG_SERIAL.println();
        delay(TIME_AFTER_MESSAGE);

        uint8_t model = dxl.getModelNumber(present_b_id);

        delay(TIME_BEFORE_MESSAGE);
        DEBUG_SERIAL.println();
        DEBUG_SERIAL.print("Model Number: "); DEBUG_SERIAL.println(model);
        DEBUG_SERIAL.println();
        delay(TIME_AFTER_MESSAGE);

        // Turn off torque when configuring items in EEPROM area
        dxl.torqueOff(present_b_id);

        if (dxl.setID(present_b_id, new_b_id) == true)
        {
            uint8_t old_b_id = present_b_id;
            present_b_id = new_b_id;

            delay(TIME_BEFORE_MESSAGE);
            DEBUG_SERIAL.println();
            DEBUG_SERIAL.print("ID has been successfully changed to "); DEBUG_SERIAL.println(new_b_id);
            DEBUG_SERIAL.println();
            delay(TIME_AFTER_MESSAGE);

            if (dxl.ping(present_b_id) == true)
            {
                delay(TIME_BEFORE_MESSAGE);
                DEBUG_SERIAL.println();
                DEBUG_SERIAL.print("ID : "); DEBUG_SERIAL.print(present_b_id); DEBUG_SERIAL.println(" :  ping succeeded!");
                DEBUG_SERIAL.println();
                delay(TIME_AFTER_MESSAGE);
            }
            else
            {
                delay(TIME_BEFORE_MESSAGE);
                DEBUG_SERIAL.println();
                DEBUG_SERIAL.print("Failed to detect ID : "); DEBUG_SERIAL.println(present_b_id);
                DEBUG_SERIAL.print("Try to detect ID : "); DEBUG_SERIAL.println(old_b_id);
                DEBUG_SERIAL.println();
                delay(TIME_AFTER_MESSAGE);

                if (dxl.ping(old_b_id) == true)
                {
                    delay(TIME_BEFORE_MESSAGE);
                    DEBUG_SERIAL.println();
                    DEBUG_SERIAL.print("ID : "); DEBUG_SERIAL.print(old_b_id); DEBUG_SERIAL.println(" :  ping succeeded!");
                    DEBUG_SERIAL.println();
                    delay(TIME_AFTER_MESSAGE);
                }
                else
                {
                    delay(TIME_BEFORE_MESSAGE);
                    DEBUG_SERIAL.println();
                    DEBUG_SERIAL.print("Failed to detect ID : "); DEBUG_SERIAL.println(old_b_id);
                    DEBUG_SERIAL.println();
                    delay(TIME_AFTER_MESSAGE);
                }
            }
        }
        else
        {
            delay(TIME_BEFORE_MESSAGE);
            DEBUG_SERIAL.println();
            DEBUG_SERIAL.print("Failed to change ID to "); DEBUG_SERIAL.println(new_b_id);
            DEBUG_SERIAL.println();
            delay(TIME_AFTER_MESSAGE);
        }
    }
    else
    {
        delay(TIME_BEFORE_MESSAGE);
        DEBUG_SERIAL.println();
        DEBUG_SERIAL.println("ping failed!");
        DEBUG_SERIAL.println();
        delay(TIME_AFTER_MESSAGE);
    }

    
    #else
    /****************************************************************
        For Model : XC430, XM540, XM430, XL320
    ****************************************************************/

    uint8_t present_id = DEFAULT_DXL_ID;
    uint8_t new_id = NEW_ID;
    dxl.begin(DEFAULT_BAUDRATE);
    dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);

        #ifdef XL320
        /****************************************************************
            For Model : XL 320 need to change baudrate
        ****************************************************************/
        
        // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
        dxl.begin(DEFAULT_BAUDRATE);
        // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
        dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);

        if(dxl.ping(present_id) == true)
        {
            delay(TIME_BEFORE_MESSAGE);
            DEBUG_SERIAL.println();
            DEBUG_SERIAL.println("ping succeeded!");
            DEBUG_SERIAL.println();
            delay(TIME_AFTER_MESSAGE);
            
            // Turn off torque when configuring items in EEPROM area
            dxl.torqueOff(present_id);
            
            // Set a new baudrate(1Mbps) for DYNAMIXEL
            dxl.setBaudrate(present_id, NEW_BAUDRATE);
            delay(TIME_BEFORE_MESSAGE);
            DEBUG_SERIAL.println();
            DEBUG_SERIAL.print("Baudrate has been successfully changed to "); DEBUG_SERIAL.println(NEW_BAUDRATE);
            DEBUG_SERIAL.print("10 sec to change baudrate monitor to"); DEBUG_SERIAL.println(NEW_BAUDRATE);
            DEBUG_SERIAL.println();
            delay(TIME_AFTER_MESSAGE);

            dxl.begin(NEW_BAUDRATE);
            delay(10000);
            
            if(dxl.ping(present_id) == true)
            {
                delay(TIME_BEFORE_MESSAGE);
                DEBUG_SERIAL.println();
                DEBUG_SERIAL.println("ping succeeded!");
                DEBUG_SERIAL.println();
                delay(TIME_AFTER_MESSAGE);
            }
            else
            {
                delay(TIME_BEFORE_MESSAGE);
                DEBUG_SERIAL.println();
                DEBUG_SERIAL.println("ping failed!");
                DEBUG_SERIAL.println();
                delay(TIME_AFTER_MESSAGE);
                // Change back to the initial baudrate
                dxl.setBaudrate(present_id, DEFAULT_BAUDRATE);
            
                delay(TIME_BEFORE_MESSAGE);
                DEBUG_SERIAL.println();
                DEBUG_SERIAL.println("Baudrate has been successfully changed back to initial baudrate");
                DEBUG_SERIAL.println();
                delay(TIME_AFTER_MESSAGE);
            }
            
        }
        else
        {
            delay(TIME_BEFORE_MESSAGE);
            DEBUG_SERIAL.println();
            DEBUG_SERIAL.println("ping failed!");
            DEBUG_SERIAL.println();
            delay(TIME_AFTER_MESSAGE);
        }
        #endif

    delay(TIME_BEFORE_MESSAGE);
    DEBUG_SERIAL.println();
    DEBUG_SERIAL.print("ID : "); DEBUG_SERIAL.println(present_id);
    DEBUG_SERIAL.println();
    delay(TIME_AFTER_MESSAGE);

    
    if (dxl.ping(present_id) == true)
    {
        delay(TIME_BEFORE_MESSAGE);
        DEBUG_SERIAL.println();
        DEBUG_SERIAL.println("ping succeeded!");
        DEBUG_SERIAL.println();
        delay(TIME_AFTER_MESSAGE);

        uint8_t model = dxl.getModelNumber(present_id);

        delay(TIME_BEFORE_MESSAGE);
        DEBUG_SERIAL.println();
        DEBUG_SERIAL.print("Model Number: "); DEBUG_SERIAL.println(model);
        DEBUG_SERIAL.println();
        delay(TIME_AFTER_MESSAGE);

        // Turn off torque when configuring items in EEPROM area
        dxl.torqueOff(present_id);

        if (dxl.setID(present_id, new_id) == true)
        {
            uint8_t old_id = present_id;
            present_id = new_id;

            delay(TIME_BEFORE_MESSAGE);
            DEBUG_SERIAL.println();
            DEBUG_SERIAL.print("ID has been successfully changed to "); DEBUG_SERIAL.println(new_id);
            DEBUG_SERIAL.println();
            delay(TIME_AFTER_MESSAGE);

            if (dxl.ping(present_id) == true)
            {
                delay(TIME_BEFORE_MESSAGE);
                DEBUG_SERIAL.println();
                DEBUG_SERIAL.print("ID : "); DEBUG_SERIAL.print(present_id); DEBUG_SERIAL.println(" :  ping succeeded!");
                DEBUG_SERIAL.println();
                delay(TIME_AFTER_MESSAGE);
            }
            else
            {
                delay(TIME_BEFORE_MESSAGE);
                DEBUG_SERIAL.println();
                DEBUG_SERIAL.print("Failed to detect ID : "); DEBUG_SERIAL.println(present_id);
                DEBUG_SERIAL.print("Try to detect ID : "); DEBUG_SERIAL.println(old_id);
                DEBUG_SERIAL.println();
                delay(TIME_AFTER_MESSAGE);

                if (dxl.ping(old_id) == true)
                {
                    delay(TIME_BEFORE_MESSAGE);
                    DEBUG_SERIAL.println();
                    DEBUG_SERIAL.print("ID : "); DEBUG_SERIAL.print(old_id); DEBUG_SERIAL.println(" :  ping succeeded!");
                    DEBUG_SERIAL.println();
                    delay(TIME_AFTER_MESSAGE);
                }
                else
                {
                    delay(TIME_BEFORE_MESSAGE);
                    DEBUG_SERIAL.println();
                    DEBUG_SERIAL.print("Failed to detect ID : "); DEBUG_SERIAL.println(old_id);
                    DEBUG_SERIAL.println();
                    delay(TIME_AFTER_MESSAGE);
                }
            }
        }
        else
        {
            delay(TIME_BEFORE_MESSAGE);
            DEBUG_SERIAL.println();
            DEBUG_SERIAL.print("Failed to change ID to "); DEBUG_SERIAL.println(new_id);
            DEBUG_SERIAL.println();
            delay(TIME_AFTER_MESSAGE);
        }
    }
    else
    {
        delay(TIME_BEFORE_MESSAGE);
        DEBUG_SERIAL.println("ping failed!");
        delay(TIME_AFTER_MESSAGE);
    }

    #endif
}

void loop()
{
    // put your main code here, to run repeatedly:
}