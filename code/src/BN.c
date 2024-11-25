#include "BN.h"
#include <stdio.h>

char BN_getpos(struct BN_Board* b_, unsigned char x, unsigned char y, unsigned char type)
{
    char retval = -1;
    if ((y < 8)&&(x<8))
    {
        if (type == BN_TYPE_SHOT) retval = (b_->shot[y]&(1<<x))>>x; //retval es solo para que de un 1 o un 0 dependiendo si hay o no un barco o un disparo
        if (type == BN_TYPE_SHIP) retval = (b_->ship[y]&(1<<x))>>x;
    }
    return retval;
}
char BN_setpos(struct BN_Board* b_, unsigned char x, unsigned char y, unsigned char type, unsigned char _bit)
{
    if (_bit > 1) _bit = 1; //APB

    char retval = -1;//CODIGO DE ERROR

    if ((y < 8)&&(x<8))// CONDICION APB
    {
        if (type == BN_TYPE_SHOT){
            if(((b_->shot[y]&(1<<x))>>x)!=_bit){ //verifica si ya esta el mismo valor a poner en la posicion
                retval = 1;
                if(_bit)
                    b_->shot[y]|=1<<x;
                else
                    b_->shot[y]&=~(1<<x);
            }

            else{
                retval = 0;
            }
        }

        else if (type == BN_TYPE_SHIP){

            if(((b_->ship[y]&(1<<x))>>x)!=_bit){
                retval = 1;
                if(_bit)
                    b_->ship[y]|=1<<x;
                else
                    b_->ship[y]&=~(1<<x);
            }
            else{
                retval = 0;
            }
        }
    }
    return retval;
}


void BN_clear_board(struct BN_Board* b_)
{
    if (b_)
    {
        uint64_t* ptr = ((uint64_t*)b_);//estoy guardando el puntero b en ptr y modifico con eso b (ptr es un puntero a int de 8bytes y shot y ship en b_ estan declarados como array de 8 chars entonces es la misma cantidad de memoria)
        *ptr = 0; //ptr esta apuntando (por como esta guardado) a shot y lo limpia
        *(ptr+1) = 0; //ptr se incrementa, y por ser de 8bytes pasa de shot a ship y lo limpia(a ship)
    }
}

void BN_set_board(struct BN_Board* b_, uint64_t shot_, uint64_t ship_)//shot y ship se mandan en hexa o decimal y se guardan en el array de 8 bytes
{
    if (b_)
    {
        uint64_t* ptr = ((uint64_t*)b_);//lo mismo que clear pero guarda lo que le pasas de shot y ship en los atributos de la variable b_
        *ptr = shot_;
        *(ptr+1) = ship_;
    }
}

void BN_print_board(struct BN_Board* b_)
{
    if (b_)
    {
        uint64_t* ptr = ((uint64_t*)b_);
        printf("%lx\n", *(ptr+1));
    }
}

void BN_change_row(struct BN_Board* b_, unsigned char y, unsigned char row, unsigned char type)//row = fila
{
    if (y < 8)
    {
        if (type == BN_TYPE_SHOT) b_->shot[y]=row;
        if (type == BN_TYPE_SHIP) b_->ship[y]=row;
    }
}

//unsigned int BN_get_mouse_pos()
//{
//    
//}