#ifndef DEBUG_H
#define DEBUG_H


/** Función para actualizar el parpadeo del LED rojo
 *  Se debe llamar periódicamente para mantener el parpadeo
 */
void debug_blink_update(void);

/** Función para iniciar el parpadeo del LED rojo
 *  @param times Cantidad de parpadeos
 */
void debug_blink(int times);

#endif // DEBUG_H
