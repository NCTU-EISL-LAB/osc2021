void uart_init ( void );
char uart_recv ( void );
void uart_send ( char c );
void uart_send_string(char* str);
int read_line(char buf[], int buf_size);
int uart_read_int(void);
void reverse(char *str,int index);
void uart_send_int(int number);
void uart_send_long(long number);

