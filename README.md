# ESF
Electronic devices Software Framework (or Extended Software Framework for electronic devices)

## Directories:
1. **components:** chip functions and registers (on [component]_io.h). Ex: ds3231.h, ds3231_io.h _//! RTC component._

2. **drivers:** middleware to better using one or more component and/or peripheral. Ex: esf_rtc.h _//! controlling internal or external RTC source, with capability to select chip._

3. **services:** better alias and usage of some drivers. Ex: esf_time.h _//! implement calendar and convert between timestamp and Date-Time structure._

4. **utils:** codes and functions and helpers (no component or driver is using by these directories). Ex: math.h

5. **config:** definetion require or optional parameters, used on other files.

6. **application:** user-side codes.

### Note:
to better design use files by defined level (on top). for example, components are using on a driver, drivers are using on a service, and services are using on application; using components on service or drivers on component are denied. exception: config, utils and *_io are free to use in anywhere.
