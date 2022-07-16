#ifndef __KNIGHTS_TOUR_H__
#define __KNIGHTS_TOUR_H__

typedef enum return_status
{
    TRUE,
    FALSE,
    TIMEOUT
} return_status_t;

return_status_t KnightsTour(unsigned char x_pos, unsigned y_pos, unsigned char *path);

return_status_t WarnsdorffKnightsTour(unsigned char x_pos, unsigned char y_pos, unsigned char *path);

#endif /*__KNIGHTS_TOUR_H__*/