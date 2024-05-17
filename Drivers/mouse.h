
#ifndef __CYANIDE__DRIVERS__MOUSE_H
#define __CYANIDE__DRIVERS__MOUSE_H

#include "../types.h"
#include "../Hardware/port.h"
#include "driver.h"
#include "../Hardware/interrupts.h"

namespace cyanide
{
    namespace drivers
    {
    
        class MouseEventHandler
        {
        public:
            MouseEventHandler();

            virtual void OnActivate();
            virtual void OnMouseDown(cyanide::common::uint8_t button);
            virtual void OnMouseUp(cyanide::common::uint8_t button);
            virtual void OnMouseMove(int x, int y);
        };
        
        
        class MouseDriver : public cyanide::hardware::InterruptHandler, public Driver
        {
            cyanide::hardware::Port8Bit dataport;
            cyanide::hardware::Port8Bit commandport;
            cyanide::common::uint8_t buffer[3];
            cyanide::common::uint8_t offset;
            cyanide::common::uint8_t buttons;

            MouseEventHandler* handler;
        public:
            MouseDriver(cyanide::hardware::InterruptManager* manager, MouseEventHandler* handler);
            ~MouseDriver();
            virtual cyanide::common::uint32_t HandleInterrupt(cyanide::common::uint32_t esp);
            virtual void Activate();
        };

    }
}
    
#endif
