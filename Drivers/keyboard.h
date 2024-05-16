
#ifndef __CYANIDE__DRIVERS__KEYBOARD_H
#define __CYANIDE__DRIVERS__KEYBOARD_H

#include "../types.h"
#include "../Hardware/interrupts.h"
#include "driver.h"
#include "../Hardware/port.h"

namespace cyanide
{
    namespace drivers
    {
    
        class KeyboardEventHandler
        {
        public:
            KeyboardEventHandler();

            virtual void OnKeyDown(char);
            virtual void OnKeyUp(char);
        };
        
        class KeyboardDriver : public cyanide::hardware::InterruptHandler, public Driver
        {
            cyanide::hardware::Port8Bit dataport;
            cyanide::hardware::Port8Bit commandport;
            
            KeyboardEventHandler* handler;
        public:
            KeyboardDriver(cyanide::hardware::InterruptManager* manager, KeyboardEventHandler *handler);
            ~KeyboardDriver();
            virtual cyanide::common::uint32_t HandleInterrupt(cyanide::common::uint32_t esp);
            virtual void Activate();
        };

    }
}
    
#endif