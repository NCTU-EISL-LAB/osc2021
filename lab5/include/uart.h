void uart_init();
void uart_write(unsigned int c);
char uart_read_raw();
char uart_read();
void uart_puts(char *s);
void uart_puts_n(char *s, int len);
void uart_flush();
int svc_uart_read(char *buf, int size);
int svc_uart_write(char *buf, int size);
