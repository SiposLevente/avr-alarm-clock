# AVR Alarm clock

This project aims to create an alarm clock with the use of an ATmega328P microcontroller.

---

## Planned Features:

- Displays the time on a 4-digit 7-segment display.
- Adjustable time with the use of buttons.
- 9 settable alarms.

## Manual: (WIP)

### Modes:

Cycling between modes happens with the press of the "mode select" button. The selected entry can be edited if the "mode button" is held for 2 seconds.

| Mode# | Mode name      | Pressing the "mode select" <2s                                            | Holding the "mode select" button >2s | Pressing "increment" button                        | Pressing "next" button                                                                      |
| ----- | -------------- | ------------------------------------------------------------------------- | ------------------------------------ | -------------------------------------------------- | ------------------------------------------------------------------------------------------- |
| 1     | Time/Date      | Switches to mode 2. In editing mode you can confirm the current settings | Edit time and date                   | In editing mode this increments the selected digit | Switches between displaying time or date. In editing mode this can cycle between the digits |
| 2     | Alarm settings | Switches to mode 1. In editing mode you can confirm the current settings | Edit selected alarm                  | In editing mode this increments the selected digit | Cycles between alarms. In editing mode this can cycle between the digits                    |

## Parts list: (WIP)

## Schematic: (WIP)
