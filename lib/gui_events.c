#include <xinu.h>
#include <gui.h>

sid32 gui_event_sem = SYSERR;

static uint32 pending_events = 0; /* bitfield of pending events */

void gui_events_init(void) {
    if (gui_event_sem == SYSERR) {
        gui_event_sem = semcreate(0);  // Start blocking
    }
}

/*Signal the GUI without specifying event type */
void gui_signal_event(void) {
    if (gui_event_sem != SYSERR) {
        signal(gui_event_sem);
    }
}

/* Signal a specific event type (ORed into pending_events) and wake GUI */
void gui_signal_event_type(uint32 event_type) {
    intmask mask = disable();
    pending_events |= event_type;
    restore(mask);
    if (gui_event_sem != SYSERR) {
        signal(gui_event_sem);
    }
}

/* Return and clear pending events atomically */
uint32 gui_get_pending_events(void) {
    uint32 ev;
    intmask mask = disable();
    ev = pending_events;
    pending_events = 0;
    restore(mask);
    return ev;
}
