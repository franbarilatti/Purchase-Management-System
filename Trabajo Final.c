#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include "gotoxy.h"
#include <windows.h>
#include <time.h>

const char archivoCliente[] = "ArchivoClientes.bin";

//ESTRUCTURAS
typedef struct //defino una estructura producto
{
    char Nombre[50];
    float precio;
    int IdProducto;
    int stock;
} producto;

typedef struct
{
    int idCliente;
    int password;
    char nombre[20];
    char apellido[20];
    char domicilio[50];
    char telefono[20];
    char mail[50];
    int altaBaja; // Alta: 1 / Baja: 0.
} cliente;

typedef struct //defino una estructura pedido
{
    char Descripcion[50];
    float Costo;
    int cantidad;
    int idPedido;
    int Estado; //alta:1 / baja:0
    char Fecha[30];
    cliente Client;
    producto Product;
} Pedido;

//FUNCIONES TIPO VOID
void StopAndClean();
void Presentacion();
void Menu_Principal();
void Registrar_Cliente();
void Menu_Ingreso();
void Menu_Administrador();
void Cargar_Stock();
void Mostrar_Lista_productos ();
void Mostrar_Producto(producto P);
void Mostrar_Producto_Stock();
void Mostrar_Producto_Sin_Stock();
void Menu_Modificacion_Producto();
void Modificacion_Stock();
void Modificacion_Precio();
void Mostrar_Nombres_Productos();
void cargarArchivoClientes(); // Carga el Archivo "Clientes".
void mostrarArchivoCliente(); // Muestra el Archivo "Clientes".
void mostrarUnCliente(cliente a); //Muestra el contenido de una estructura tipo cliente.
void mostrarIDsCargadas(); // Muestra las IDs existentes en el archivo
void iniciarSesion(); //Funcion Para Iniciar Sesion
void menuCliente(cliente c); //Menu de opciones para el cliente
void Cargar_Archivo_Pedido(cliente C);
void Mostrar_Archivo_Pedidos();
void Mostrar_Pedido(Pedido Pe);
void Fecha(char fechayhora[30]);
void Dar_Baja_Pedido(cliente C);
void Mostar_Pedidos_Activos();
void Mostar_Pedidos_Cancelados();
void Mostrar_Pedidos_Personales(cliente C);
void modificarCliente(cliente c);//Funcion que toma un dato tipo cliente del archivo y modifica sus datos
void clientesDeAlta();
void clientesDeBaja();
void Menu_Inicio_Sesion();
void clientesDeAlta();
void clientesDeBaja();
void Firma_Digital();
void darAltaCliente(cliente c);

//FUNCIONES CON RETORNO
int Contar_Cantidad_Productos();
int Buscar_Pos_Producto(int NP);
float Ingresar_Precio();
int compararIDs(int ID); // Funcion que Devuelve un 1 si el ID Cargado Ya Esta en el Archivo
cliente buscarCliente(); //Funcion Que Busca un Cliente Por su Numero de ID
int escaneoSesion(int flag, cliente *Clien); //Funcion Para Escanear los datos del Inicio de Sesion
int Cantidad_Producto();
int Buscar_Producto(int buscado);
int BuscarPosicionCliente(int ID);//Funcion que devuelve un entero igual a la posicion de un cliente en el archivo tomando por parametro su ID
int contarCantidadClientes();//Funcion que devuelve un entero igual a la cantidad de clientes registrados en el archivo
int Contar_Cantidad_Pedidos();
float Saldo_Cliente(cliente c);

//MAIN

int main()
{
    Menu_Principal();
    system("cls");
    Firma_Digital();

    return 0;
}
void StopAndClean() //cuando es invocada pausa y limpia la pantalla
{

    system("pause>null");
    system("cls");
}

void Presentacion()  //muestra por pantalla un titulo
{
    int x=30,y=2;
    gotoxy(x,y);
    color(03);
    printf("BIENVENIDOS AL SISTEMA DE GESTION DE COMPRAS");
    color(07);
}

void Menu_Principal() //menu desde el cual se gestionan las diferentes operaciones del programa
{

    int salida=1,op;
    while(salida!=0)
    {
        Presentacion();
        printf("\n\n Seleccione una de las siguientes opciones (ingrese el numero y presione ENTER)\n\n");
        printf("[1] Registrarse\n[2] Ingresar\n\n\n[0] Salir\n\n");
        fflush(stdin);
        scanf("%d",&op);
        switch(op)
        {
        case 1:
            cargarArchivoClientes();
            system("cls");
            break;
        case 2:
            Menu_Inicio_Sesion();
            system("cls");
            break;
        case 2422:
            Menu_Administrador(); //el usuario debera ingresar una clave que solo sabra el administrador "9821"
            //llama a la funcion menu administrador

            break;

        case 0:
            salida--;  //ingresando 0 sale del programa
            break;
        default:     //si se ingresa una opcion no valida lo avisa por pantalla
            printf("\n\n Ha ingresado una opcion invalida\n\n");
            StopAndClean();
            break;

        }

    }
}



//------------------------------STOCK-------------------------------------------


void Cargar_Stock()  //carga productos al archivo de stock
{
    system("cls");
    producto p;
    FILE *Arch=fopen("stock","ab"); // creo el archivo stock
    fseek(Arch,0,SEEK_END);
    int i;
    fflush(stdin);
    char seguir='s';
    if(Arch!=NULL)
    {
        while(seguir=='s')
        {
            i=ftell(Arch)/sizeof(producto);// le asigno i un valor igual a la posicion del producto
            color(03);
            printf("\n\t\t\t\t\tSECCION DE GESTION DE STOCK\n\n\n");
            color(07);
            printf(" Producto: ");
            fflush(stdin);
            gets(p.Nombre); //ingreso el nombre del producto
            p.precio=Ingresar_Precio(); //le asigno un precio
            printf("Stock: ");
            scanf("%d", &p.stock); //cargo la cantidad disponible del producto
            p.IdProducto=i+1; //le asigno a la id del producto el valor de i+1 para que empiece en 1
            fwrite(&p,sizeof(producto),1,Arch); // cargo los datos al archivo productos
            printf("\n Quiere seguir?\n"); // si el usuario quiere seguir se vuelve a ingresar en el ciclo
            fflush(stdin);
            scanf("%c", &seguir);
            system("cls");
        }
        fclose(Arch); //cierro el archivo para que se guarden los datos
    }
}

void Mostrar_Lista_productos () //muestra una lista con todos los productos
{
    system("cls");
    color(03);
    printf("\n\t\t\t\t\tLISTA DE PRODUCTOS\n\n\n");
    color(07);
    FILE *Arch=fopen("stock","rb");
    producto Aux;
    if (Arch!=NULL)
    {
        while(fread(&Aux,sizeof(producto),1,Arch)>0)
        {
            Mostrar_Producto(Aux);

        }
        fclose(Arch);
    }
    else
    {
        printf("No hay ninguna lista creada\n");
    }

}
void Mostrar_Producto(producto P) //muestra los datos de un producto
{
    puts("---------------------------------------");
    printf("Producto: ");
    puts(P.Nombre);
    printf("\n Precio: $%.2f",P.precio);
    printf("\n\n Numero de producto: %d",P.IdProducto);
    printf("\n\n Stock disponible: %d\n",P.stock);
    puts("---------------------------------------");
}

void Menu_Administrador() // muestra un menu accesible unicamente para el administrador
{
    cliente  Aux;
    int salida=1;
    while(salida!=0)
    {
        system("cls");
        color(03);
        gotoxy(35,2);
        printf("MENU ADMINISTRADOR\n");
        color(07);
        printf("Que desea hacer?\n\n");
        printf("[1] Cargar cliente\n");
        printf("[2] Modificar datos de un cliente\n");
        printf("[3] Dar alta cliente\n");
        printf("[4] Ver lista de clietes\n");
        printf("[5] Ver lista de clietes activos\n");
        printf("[6] Ver lista de clietes dados de baja\n");
        printf("[7] Ver inventario\n");
        printf("[8] Ver productos en stock\n");
        printf("[9] Ver productos sin stock\n");
        printf("[10] Modificar productos\n");
        printf("[11] Cargar productos\n");
        printf("[12] Ver todos los pedidos\n");
        printf("[13] Ver pedidos activos\n");
        printf("[14] Ver pedidos cancelados\n");
        printf("\n[0] Menu anterior\n");
        int op;
        fflush(stdin);
        scanf("%d",&op);
        switch(op)
        {
        case 1:
            cargarArchivoClientes();
            system("cls");
            break;
        case 2:
            Aux=buscarCliente();
            modificarCliente(Aux);
            system("cls");
            break;
        case 3:
            Aux=buscarCliente();
            darAltaCliente(Aux);
            system("cls");
            break;
        case 4:
            mostrarArchivoCliente();
            StopAndClean();
            break;
        case 5:
            clientesDeAlta();
            StopAndClean();
            break;
        case 6:
            clientesDeBaja();
            StopAndClean();
            break;
        case 7:
            Mostrar_Lista_productos(); //llama a la funcion mostrar lista productos
            StopAndClean();
            break;
        case 8:
            Mostrar_Producto_Stock(); //llama a la funcion mostrar producto stock
            StopAndClean();
            break;
        case 9:
            Mostrar_Producto_Sin_Stock(); // llama a la funcion mostrar producto sin stock
            StopAndClean();
            break;
        case 10:
            Menu_Modificacion_Producto(); //llama a la funcion menu modificacion producto
            StopAndClean();
            break;
        case 11:
            Cargar_Stock(); //llama a la funcion cargar stock
            system("cls");
            break;
        case 12:
            Mostrar_Archivo_Pedidos();
            StopAndClean();
            break;
        case 13:
            Mostar_Pedidos_Activos();
            StopAndClean();
            break;
        case 14:
            Mostar_Pedidos_Cancelados();
            StopAndClean();
            break;
        case 0:
            salida--; // hace 0 a salida para cortar el ciclo
            system("cls");
            break;
        default:
            printf("Opcion ingresada invalida\n\n"); // si se ingresa una opcion no valida lo muestra por pantalla
            StopAndClean();
            break;
        }
    }
}

void Mostrar_Producto_Stock() //muestra los productos que tienen un stock mayor a 0
{
    system("cls");
        color(03);
        gotoxy(35,2);
        printf("PRODUCTOS CON STOCK\n\n");
        color(07);
    FILE *Arch=fopen("stock","rb");
    producto P;
    if (Arch!=NULL)
    {
        while(fread(&P,sizeof(producto),1,Arch)>0)
        {
            if(P.stock>0)  // uso la condicion p.stock>0 para que solo se muestren los productos que tengan stock
            {
                Mostrar_Producto(P); //llamo a la funcion mostrar producto
            }
        }
    }
}
void Mostrar_Producto_Sin_Stock() //muestra los productos su stock sea igual a 0
{
    system("cls");
        color(03);
        gotoxy(35,2);
        printf("PRODUCTOS SIN STOCK\n\n");
        color(07);
    FILE *Arch=fopen("stock","rb");
    producto P;
    if (Arch!=NULL)
    {
        while(fread(&P,sizeof(producto),1,Arch)>0)
        {
            if(P.stock==0) // uso la condicion p.stock para mostrar solo los productos sin stock
            {
                Mostrar_Producto(P);
            }
        }
    }
}

int Buscar_Pos_Producto(int NP) // busca un producto por su nombre y devielve la posicion
{
    int pos=1;
    producto p;
    FILE *Arch=fopen("stock","rb");
    if(Arch!=NULL)
    {
        while(fread(&p,sizeof(producto),1,Arch)>0 && NP!=p.IdProducto) //el ciclo se corta cuando se llega al final del archivo o se encuentra el nombre
        {
            pos++;
        }
        fclose(Arch);
    }

    return pos;
}

void Menu_Modificacion_Producto() //menu donde se puede elegir las distintas opciones de modificacion un producto
{
    int salida=0;
    while(salida!=1)
    {
        system("cls");
        color(03);
        gotoxy(35,2);
        printf("MENU GESTION DE PRODUCTOS\n");
        color(07);
        printf("Que desea hacer?\n\n");
        printf("[1]  Modificar stock de un producto\n");
        printf("[2]  Modificar precio de un producto\n\n");
        printf("\n[0]  Menu anterior\n");
        int op;
        fflush(stdin);
        scanf("%i",&op) ;
        switch(op)
        {
        case 1:
            Modificacion_Stock(); //llama a la funcion modificacion stock
            StopAndClean();
            break;
        case 2:
            Modificacion_Precio();
            StopAndClean();
            break;
        case 0:
            salida++; // incrementa salida a 1 para salir del ciclo
            break;
        default:
            printf("Opcion ingresada no valida\n");// si el usuario ingresa una opcion no valida lo muestra por pantalla
            break;
        }

    }

}

void Modificacion_Stock()  // modifica el stock de un producto igresado por teclado
{
    system("cls");
    int pos = 0;
    int cantidadP=Contar_Cantidad_Productos();//le asigno a cantidadP la cantidad de productos cargados
    producto P;
    printf("\n\nIngrese el numero de producto que quiere agregar stock\n");
    Mostrar_Nombres_Productos();
    int Nproducto;
    fflush(stdin);
    scanf("%d",&Nproducto);
    pos = Buscar_Pos_Producto(Nproducto); // busca la posicion del producto ingresado
    FILE *Arch= fopen("stock", "r+b");
    if(Arch != NULL && pos<cantidadP) //si el archivo se abre correctamente y si la posicion del producto no supera la cantidad de productos
    {
        fseek(Arch,(pos-1)*sizeof(producto),SEEK_SET); //vuelvo una posicion hacia atras
        fread(&P,sizeof(producto),1,Arch); //leo el producto
        printf("Stock actual: %d\n\n",P.stock);
        printf("Quiere modificar el stock del producto? S/N ");
        char op;
        fflush(stdin);
        scanf("%c",&op);
        if (op=='s') //si el usuario quiere modificar el stock
        {
            fseek(Arch,(pos-1)*sizeof(producto),SEEK_SET); //vuelvo a ir una posicion hacia atras
            printf("\n\nIngrese la cantidad que quiere agregar: ");
            int cant=0;
            fflush(stdin);
            scanf("%d",&cant); //ingreso la cantidad que quiero agregar o quitar al stock
            P.stock+=cant; //le agrego esa cantidad al stock
            fwrite(&P,sizeof(producto),1,Arch); //lo escrivo en el archivo
            printf("\n\n Nuevo stock: %d",P.stock); //muestro el nuevo stock del producto
        }
        else
        {
            printf("El stock no ha sido modificado\n\n"); // si el usuario no quiere modificar el producto
        }
        fclose(Arch);
    }
    else
    {
        printf("Producto no encontrado\n"); //si el nombre ingresado no se encuentra en la lista se avisa por pantalla
    }
}

void Modificacion_Precio()  // modifica el stock de un producto igresado por teclado
{
    system("cls");
    int pos = 0;
    int cantidadP=Contar_Cantidad_Productos();//le asigno a cantidadP la cantidad de productos cargados
    producto P;
    printf("\n\nIngrese el numero de producto que quiere modificar el precio\n");
    Mostrar_Nombres_Productos();
    int Nproducto;
    fflush(stdin);
    scanf("%d",&Nproducto);
    pos = Buscar_Pos_Producto(Nproducto); // busca la posicion del producto ingresado
    FILE *Arch= fopen("stock", "r+b");
    if(Arch != NULL && pos<cantidadP) //si el archivo se abre correctamente y si la posicion del producto no supera la cantidad de productos
    {
        fseek(Arch,(pos-1)*sizeof(producto),SEEK_SET); //vuelvo una posicion hacia atras
        fread(&P,sizeof(producto),1,Arch); //leo el producto
        printf("Precio actual: %.2f\n\n",P.precio);
        printf("Quiere modificar el precio del producto? S/N ");
        char op;
        fflush(stdin);
        scanf("%c",&op);
        if (op=='s') //si el usuario quiere modificar el stock
        {
            fseek(Arch,(pos-1)*sizeof(producto),SEEK_SET); //vuelvo a ir una posicion hacia atras
            printf("\n\nIngrese el nuevo precio");
            P.precio=Ingresar_Precio(); //ingreso el nuevo precio
            fwrite(&P,sizeof(producto),1,Arch); //lo escrivo en el archivo
            printf("\n\n Nuevo precio: %.2f",P.precio); //muestro el nuevo precio del producto
        }
        else
        {
            printf("El precio no ha sido modificado\n\n"); // si el usuario no quiere modificar el producto
        }
        fclose(Arch);
    }
    else
    {
        printf("Producto no encontrado\n"); //si el nombre ingresado no se encuentra en la lista se avisa por pantalla
    }
}
int Contar_Cantidad_Productos() // cuenta y retorna la cantidad de productos cargados
{
    FILE *Arch=fopen("stock","rb");
    producto p;
    int i=1;
    if(Arch!=NULL)
    {
        while(fread(&p,sizeof(producto),1,Arch)>0)
        {
            i++;
        }
        fclose(Arch);
    }
    return i;
}

void Mostrar_Nombres_Productos() // muesta los nombres de los productos cargados y en que posicion

{
    producto p;
    FILE *Arch=fopen("stock","rb");
    int i=1;
    if(Arch!=NULL)
    {
        while(fread(&p,sizeof(producto),1,Arch)>0)
        {
            printf("[%d]: ",i);
            puts(p.Nombre);
            i++;
        }
        fclose(Arch);
    }

}

float Ingresar_Precio()
{
    int salida=0;
    float precio;
    printf(" Precio : $");
    scanf("%f",&precio);
    while(precio<0 && salida==0)
    {
        printf(" Precio ingresado invalido. Por favor ingrese uno nuevo\n");
        printf(" Precio : $");
        scanf("%f",&precio);
    }
    return precio;
}



//-------------------------------CLIENTES-------------------------------

void mostrarIDsCargadas() //Funcion Que Muestra por Pantalla Las ID que Estan Cargadas en el Archivo
{
    cliente c;
    FILE*archi = fopen(archivoCliente,"rb");
    printf("\nIDs ya cargados en el sistema: ");
    if(archi!=NULL)
    {
        while(fread(&c,sizeof(cliente),1,archi));
        {

            printf("   |%i|   ",c.idCliente);
        }
    }
}


int compararIDs(int ID) // Funcion que devuelve un 1 si el id cargado ya esta en el archivo
{
    cliente c;
    int flag = 0;
    FILE*archi = fopen(archivoCliente,"rb");
    if(archi!=NULL);
    {
        while(fread(&c,sizeof(cliente),1,archi)>0 && flag == 0 )
        {
            if(c.idCliente == ID)
            {
                flag = 1;
            }
        }
        fclose(archi);
    }
    return flag;
}

void cargarArchivoClientes() // Carga el Archivo "Clientes".
{
    char control = 's';
    cliente a;
    FILE *archi;

    archi = fopen(archivoCliente, "ab");

    if(archi != NULL)
    {
        while(control == 's')
        {
            system("cls");
            color(03);
            gotoxy(35,2);
            printf("GESTION DE CARGA DE CLIENTE\n");
            color(07);
            printf("\n\nIngrese ID Cliente (5 digitos, cero para ir al menu anterior): "); // ID.
            fflush(stdin);
            scanf("%i", &a.idCliente);
            if (a.idCliente!=0)
            {
                int retorno = compararIDs(a.idCliente);// Creo una variable igualada a la funcion compararIDs
                if(retorno == 0  )// Si retorno es igual a 0 le pide al cliente que siga cargando sus datos
                {
                    printf("Ingrese una Contraseña de 4 digitos: ");
                    fflush(stdin);
                    scanf("%i",&a.password);
                    printf("Ingrese Nombre: "); // Nombre.
                    fflush(stdin);
                    gets(a.nombre);
                    printf("Ingrese Apellido: "); // Apellido.
                    fflush(stdin);
                    gets(a.apellido);
                    printf("Ingrese Domicilio: "); // Domicilio.
                    fflush(stdin);
                    gets(a.domicilio);
                    printf("Ingrese Telefono: "); // Telefono.
                    fflush(stdin);
                    gets(a.telefono);
                    printf("Ingrese E-Mail: "); // Mail.
                    fflush(stdin);
                    gets(a.mail);
                    a.altaBaja=1; // Alta / Baja.


                    puts("-----------------------------\n");

                    fwrite(&a, sizeof(cliente), 1, archi);
                    printf("\nDesea cargar otro cliente? S/N: ");
                    fflush(stdin);
                    scanf("%c", &control);
                }
                else // Si retorno es distinto de 0 quiere decir que la ID que se esta queriendo ingresar ya esta usada
                {
                    printf("\nERROR: el cliente que esta queriendo registrar ya esta ingresado en el sistema.");
                    printf("\nPor favor ingrese de nuevo los datos del cliente. \n\n");
                    StopAndClean();
                }

            }
            else
            {
                control='n';
            }
            fclose(archi);
        }
    }

}

void mostrarArchivoCliente() // Muestra el Archivo "Clientes".
{
    cliente a;
    FILE *archi;

    archi = fopen(archivoCliente, "rb");

    if(archi != NULL)
    {
        while(fread(&a, sizeof(cliente), 1, archi) > 0)
        {
            mostrarUnCliente(a);
        }
        fclose(archi);
    }
    printf("\n\n");
}

void mostrarUnCliente(cliente a) // mostrar datos del cliente por pantalla
{

    puts("-------------------------\n");
    printf("ID Cliente: %i", a.idCliente);
    printf("\nNombre: %s", a.nombre);
    printf("\nApellido: %s", a.apellido);
    printf("\nDomicilio: %s", a.domicilio);
    printf("\nTelefono: %s", a.telefono);
    printf("\nE-Mail: %s", a.mail);
    if(a.altaBaja==1)
    {
        printf("\nCliente activo");
    }
    else
    {
        printf("\nCliente inactivo");
    }

    puts("\n-------------------------\n");
}

cliente buscarCliente() //Funcion Que Busca un Cliente Por su Numero de ID
{
    cliente c;
    int flag = 0;
    int ID;
    FILE*archi = fopen(archivoCliente,"rb");
    printf("Ingrese la ID del cliente que quierea buscar: ");
    fflush(stdin);
    scanf("%i",&ID);
    if(archi!=NULL)
    {
        while(fread(&c,sizeof(cliente),1,archi) > 0  && flag == 0)
        {
            if(c.idCliente == ID)// Si la ID cargada es igual a la de algun cliente en el archivo, retorna el cliente en cuestion
            {
                fseek(archi,(-1)*sizeof(cliente),SEEK_CUR);
                mostrarUnCliente(c);
                flag = 1;// Modifico el valor de flag
                printf("\n\n");
                StopAndClean();
            }

        }
        fclose(archi);
    }
    if(flag == 0)// Si flag sigue valiendo 0 quiere decir que el cliente no esta en el archivo
    {
        printf("\n\nEl Cliente no esta registrado.\n\n\n");
        StopAndClean();
    }
    return c;
}

void Menu_Inicio_Sesion()
{
    system("cls");
    color(03);
    gotoxy(35,2);
    printf("BIENVENIDO\n");
    color(07);
    int op, z=0;
    printf("[1]  Iniciar sesion\n\n[0]  Menu anterior\n");
    scanf("%d",&op);
    while(z==0)
    {
        switch(op)
        {

        case 1:
            iniciarSesion();
            system("cls");
            z++;
            break;
        case 0:
            z++;
            break;
        default:
            printf("Opcion invalida.Ingrese una nueva opcion\n");
            fflush(stdin);
            scanf("%d",&op);
            break;

        }
    }

}

int escaneoSesion(int flag, cliente *Clien) //Funcion Para Escanear los datos del Inicio de Sesion
{
    cliente c;
    int IDaux=0;

    printf("\n\n\nIniciar sesion\n");
    printf("\nID:");
    fflush(stdin);
    scanf("%i",&IDaux);
    FILE*archi = fopen(archivoCliente,"rb");
    if(archi!=NULL)
    {
        while(fread(&c,sizeof(cliente),1,archi)>0 && flag == 0)
        {
            if(IDaux == c.idCliente)
            {

                if(c.altaBaja == 1)
                {
                    int password = 0;
                    printf("Contrase%ca: ",164);
                    fflush(stdin);
                    scanf("%i",&password);
                    if(password == c.password)
                    {
                        flag = 1;
                        printf("\n\n Bienvenido %s %s\n\n",c.nombre, c.apellido);
                        StopAndClean();
                        *Clien=c;
                    }
                    else
                    {
                        printf("\nLa Contrase%ca es Incorrecta. ",164);
                        flag = -1;

                    }
                }
                else
                {
                    printf("\nEl Usuario al que Esta Queriendo Ingresar Esta Dado de Baja");
                    StopAndClean();
                    flag = -1;

                }
            }


        }
        fclose(archi);
    }
    if(flag == 0)
    {
        printf("\nEl ID ingresado no esta registrado en el sistema.\n");
        printf("\nVuleva a intentarlo\n\n");
        StopAndClean();
    }

    return flag;
}
void iniciarSesion() //Funcion Para Iniciar Sesion
{
    cliente c;
    int flag = 0;

    char control;
    int intentos = 3;

    while(intentos != 0)
    {
        flag = escaneoSesion(flag,&c);
        if(flag == (-1))
        {
            printf("\n\nLe quedan %i intentos, Desea continuar? S/N: ",intentos-1);
            fflush(stdin);
            scanf("%c",&control);
            if(control == 's')
            {
                flag = 0;
                intentos--;
                if (intentos==0)
                {
                    flag = 0;
                    printf("\nSe ha quedado sin intentos. Consulte con el administrador.\n\n");
                    system("pause>null");
                }
            }
            else
            {
                printf("Gracias, vuelva pronto.");
                intentos = 0;
            }


        }
        else if(flag == 1)
        {

            menuCliente(c);
            intentos = 0;

        }

    }




}

void menuCliente(cliente c)// Funcion de menu para el cliente
{
    system("cls");

    int z = 0;
    int opc;
    while(z == 0)

    {
        color(03);
        gotoxy(35,2);
        printf("MENU CLIENTE\n");
        color(07);
        printf("\n\n[1]- Hacer Pedido\n[2]- Ver Mis Pedidos\n[3]- Modificar Informacion Personal\n[4]- Ver Datos Personales\n[5]- Cancelar pedido\n\n[0]- Menu anterior  ");
        printf("\n\nIngresar una opcion: ");
        scanf("%i",&opc);
        switch (opc)
        {
        case 1:
            Cargar_Archivo_Pedido(c);
            system("cls");
            break;
        case 2:
            Mostrar_Pedidos_Personales(c);
            StopAndClean();
            break;
        case 3:
            modificarCliente(c);
            StopAndClean();
            break;
        case 4:
            mostrarUnCliente(c);
            StopAndClean();
            break;
        case 5:
            Dar_Baja_Pedido(c);
            StopAndClean();
            break;
        case 0:
            z++;

            break;
        }
    }

}

void modificarCliente(cliente c)  // Modifica los datos del cliente
{
    system("cls");
    int pos = 0;
    int cantidadC=contarCantidadClientes();//le asigno a cantidadP la cantidad de productos cargados

    pos = BuscarPosicionCliente(c.idCliente); // busca la posicion del producto ingresado
    FILE *Arch= fopen(archivoCliente, "r+b");
    if(Arch != NULL && pos<cantidadC) //si el archivo se abre correctamente y si la posicion del producto no supera la cantidad de productos
    {
        int z = 0;
        int opc;
        char op;
        while(z == 0)

        {

            fseek(Arch,(pos-1)*sizeof(cliente),SEEK_SET); //vuelvo una posicion hacia atras
            fread(&c,sizeof(cliente),1,Arch); //leo el producto
            printf("\nElige la opcion que desea modificar");
            printf("\n\n[1]- Domicilio\n[2]- Telefono\n[3]- E-Mail\n[4]- Darse de Baja\n\n[0]- Salir  ");
            printf("\n\nIngresar una opcion: ");
            scanf("%i",&opc);
            switch (opc)
            {
            case 1:
                printf("Domicilio actual: %s\n\n",c.domicilio);
                printf("Quiere modificar el Domicilio?  S/N:");
                fflush(stdin);
                scanf("%c",&op);
                if (op=='s') //si el usuario quiere modificar el stock
                {
                    fseek(Arch,(pos-1)*sizeof(cliente),SEEK_SET); //vuelvo a ir una posicion hacia atras
                    printf("\n\nIngrese el nuevo Domicilio: ");
                    fflush(stdin);
                    gets(c.domicilio); //ingreso el nuevo precio
                    fwrite(&c,sizeof(cliente),1,Arch); //lo escrivo en el archivo
                    printf("\n\n Nuevo Domicilio: %s\n",c.domicilio); //muestro el nuevo precio del producto
                }
                else
                {
                    printf("La Direccion no ha sido modificada\n\n"); // si el usuario no quiere modificar el producto
                }
                StopAndClean();
                break;
            case 2:
                printf("Telefono actual: %s\n\n",c.telefono);
                printf("Quiere modificar el numero de Telefono?  S/N:");
                fflush(stdin);
                scanf("%c",&op);
                if (op=='s') //si el usuario quiere modificar el stock
                {
                    fseek(Arch,(pos-1)*sizeof(cliente),SEEK_SET); //vuelvo a ir una posicion hacia atras
                    printf("\n\nIngrese el nuevo numero de Telefono \n");
                    fflush(stdin);
                    gets(c.telefono); //ingreso el nuevo precio
                    fwrite(&c,sizeof(cliente),1,Arch); //lo escrivo en el archivo
                    printf("\n\n Nuevo telefono: %s\n",c.telefono); //muestro el nuevo precio del producto
                }
                else
                {
                    printf("El numero de Telefono no ha sido modificado\n\n"); // si el usuario no quiere modificar el producto
                }
                StopAndClean();
                break;
            case 3:
                printf("\nE-Mail actual actual: %s\n\n",c.mail);
                printf("Quiere modificar la direccion de E-Mail?  S/N:");

                fflush(stdin);
                scanf("%c",&op);
                if (op=='s') //si el usuario quiere modificar el stock
                {
                    fseek(Arch,(pos-1)*sizeof(cliente),SEEK_SET); //vuelvo a ir una posicion hacia atras
                    printf("\n\nIngrese la nueva direccion de E-Mail \n");
                    fflush(stdin);
                    gets(c.mail); //ingreso el nuevo precio
                    fwrite(&c,sizeof(cliente),1,Arch); //lo escrivo en el archivo
                    printf("\n\n Nueva direccion de E-Mail: %s\n",c.mail); //muestro el nuevo precio del producto
                }
                else
                {
                    printf("\nLa direccion de E-Mail no ha sido modificada\n\n"); // si el usuario no quiere modificar el producto
                    StopAndClean();
                }
                break;
            case 4:


                printf("\n\nQuiere darse de Baja del sistema?  S/N:");
                fflush(stdin);
                scanf("%c",&op);
                if (op=='s') //si el usuario quiere modificar el stock
                {
                    fseek(Arch,(pos-1)*sizeof(cliente),SEEK_SET); //vuelvo a ir una posicion hacia atras
                    printf("\n\nSi quiere dar de baja su cuenta ingrese 0: ");
                    fflush(stdin);
                    scanf("%i",&c.altaBaja);
                    if(c.altaBaja==0)
                    {
                        printf("\n\n Gracias por Haber Confiado en Nuestro Sistema, Esperamos Su Regreso.\n");
                    }
                    else
                    {
                        c.altaBaja=1;
                        printf("\nEl Usuario No Ha Sido Dado de Baja. Intentelo de Nuevo\n\n");
                    }
                    fwrite(&c,sizeof(cliente),1,Arch); //lo escrivo en el archivo
                }

                StopAndClean();
                break;
            case 0:
                z++;

                break;
            }
        }

    }
    else
    {
        printf("Cliente no encontrado\n"); //si el nombre ingresado no se encuentra en la lista se avisa por pantalla
    }
}

int contarCantidadClientes() // Cuenta y retorna la cantidad de clientes cargados
{
    FILE *Arch=fopen(archivoCliente,"rb");
    cliente c;
    int i=1;
    if(Arch!=NULL)
    {
        while(fread(&c,sizeof(cliente),1,Arch)>0)
        {
            i++;
        }
        fclose(Arch);
    }
    return i;
}

int BuscarPosicionCliente(int ID) // busca un producto por su nombre y devielve la posicion
{
    int pos=1;
    cliente c;
    FILE *Arch=fopen(archivoCliente,"rb");
    if(Arch!=NULL)
    {
        while(fread(&c,sizeof(cliente),1,Arch)>0 && ID!=c.idCliente) //el ciclo se corta cuando se llega al final del archivo o se encuentra el nombre
        {
            pos++;
        }
        fclose(Arch);
    }

    return pos;
}

void clientesDeAlta() // Funcion que muestra los clientes dados de Alta
{
    cliente c;
    FILE *archi = fopen(archivoCliente,"rb");
    if(archi!=NULL)
    {
        while(fread(&c,sizeof(cliente),1,archi)>0) // Recorro el archivo
        {
            if(c.altaBaja == 1) // Si el altaBaja del cliente es igual a 1
            {
                mostrarUnCliente(c); // Muestra todos los clientes dados de Alta
            }
        }
        fclose(archi);
    }
}

void clientesDeBaja() // Funcion que muestra los clientes dados de Baja
{
    cliente c;
    FILE *archi = fopen(archivoCliente,"rb");
    if(archi!=NULL)
    {
        while(fread(&c,sizeof(cliente),1,archi)>0) // Recorro el archivo
        {
            if(c.altaBaja == 0) // Si el altaBaja del cliente es igual a 0
            {
                mostrarUnCliente(c); // Muestra todos los clientes dados de Baja
            }
        }
        fclose(archi);
    }
}


void darAltaCliente(cliente c) //Da de alta un cliente que esta dado de baja
{
    system("cls");
    int pos = 0;
    char op='s';
    int cantidadC=contarCantidadClientes();

    pos = BuscarPosicionCliente(c.idCliente);
    FILE *Arch= fopen(archivoCliente, "r+b");
    if(Arch != NULL && pos<cantidadC)
    {
        printf("Quiere de alta a un cliente?  S/N:");
        fflush(stdin);
        scanf("%c",&op);
        if (op=='s')
        {
            fseek(Arch,(pos-1)*sizeof(cliente),SEEK_SET);
            c.altaBaja=1;
            fwrite(&c,sizeof(cliente),1,Arch);
            printf("\n\n El usuaria ha sido dado de alta exitosamente\n");
        }
        else
        {

            printf("\nEl Usuario no ha sido dado de alta\n\n");
        }

        fclose(Arch);
    }
    StopAndClean();
}



//------------------------------------------- PEDIDOS-----------------------------------------------//


void Cargar_Archivo_Pedido(cliente C) //Carga un pedido al archivo
{
    int i;
    Pedido Pe;
    producto Pro;
    system("cls");
    color(03);
    gotoxy(35,2);
    printf("GENERADOR DE PEDIDOS\n");
    color(07);
    int eleccion;
    char seguir='s';
    FILE *ArchiPedido=fopen("pedidos","ab");
    FILE *ArchiStock=fopen("stock","r+b");
    if (ArchiPedido!=NULL && ArchiStock!=NULL)
    {

        int cant;
        printf("Quiere generar un pedido? S/N ");
        fflush(stdin);
        scanf("%c",&seguir);
        i=Contar_Cantidad_Pedidos();
        while(seguir=='s')
        {
            i++;
            Mostrar_Lista_productos();
            printf("Ingrese el numero del producto que quiere comprar: \n");
            scanf("%d",&eleccion);
            int pos=Buscar_Pos_Producto(eleccion);
            int control=Buscar_Producto(eleccion);
            if(control==1)
            {
                fseek(ArchiStock,(pos-1)*sizeof(producto),SEEK_SET);
                fread(&Pro,sizeof(producto),1,ArchiStock);
                if(Pro.stock!=0)
                {
                    Pe.Client=C;
                    cant=Cantidad_Producto(eleccion);
                    float total=(float)(Pro.precio*cant);
                    Pe.Costo=total;
                    Pe.Product.IdProducto=eleccion;
                    Pe.idPedido=i;
                    Fecha(Pe.Fecha);
                    strcpy(Pe.Descripcion,Pro.Nombre);
                    Pe.cantidad=cant;
                    printf("Confirmar pedido? S/N \n");
                    char resp;
                    fflush(stdin);
                    scanf("%c",&resp);
                    if(resp=='s')
                    {
                        Pro.stock=Pro.stock-cant;

                        Pe.Estado=1;
                        fwrite(&Pe,sizeof(Pedido),1,ArchiPedido);
                        fseek(ArchiStock,(pos-1)*sizeof(producto),SEEK_SET);
                        fwrite(&Pro,sizeof(producto),1,ArchiStock);
                    }
                    else
                    {
                        printf("Esta seguro? S/N\n");
                        scanf("%c",&resp);
                        if(resp=='n')
                        {
                            Pe.Estado=1;
                            fwrite(&Pe,sizeof(Pedido),1,ArchiPedido);
                        }
                    }
                }
                else
                {
                    printf("Actualmente no poseemos stock del producto elegido\n");
                }
            }
            else
            {
                printf("El producto no se encuentra disponible\n");
                system("pause>null");
                Cargar_Archivo_Pedido(C);
            }
            printf("Quiere hacer otro pedido? S/N\n");
            fflush(stdin);
            scanf("%c",&seguir);
        }

        fclose(ArchiPedido);
        fclose(ArchiStock);

    }
}

int Buscar_Producto(int buscado) //Busca un producto y retorna un flag por si esta o no
{
    FILE *Arch=fopen("stock","rb");
    int flag=0;
    producto P;
    if(Arch!=NULL)
    {
        while(fread(&P,sizeof(producto),1,Arch)&& flag==0)
        {
            if (P.IdProducto==buscado)
            {
                flag=1;
            }
        }
        fclose(Arch);
    }
    return flag;
}

int Cantidad_Producto(int eleccion) //Controla que haya stock suficiente para un pedido
{
    int cant=0,salida;
    producto Pro;
    FILE *Arch=fopen("stock","rb");
    if(Arch!=NULL)
    {
        printf("Cantidad que quiere: ");
        scanf("%d",&cant); //Leo una cantidad que quiere pedir el usuario
        fseek(Arch,(eleccion-1)*sizeof(producto),SEEK_SET);
        fread(&Pro,sizeof(producto),1,Arch);

        salida=Pro.stock-cant; //Le asigno a salida la diferencia entre el stock y la cantidad pedida

        while(cant<=0 || salida<0) // Mientras el usuario ingrese un valor menor igual a 0 o que haga a salida menor a 0 pide ingresar una cantidad nueva
        {
            printf("Cantidad no valida. Ingrese una nueva cantidad: ");
            scanf("%d",&cant);
            salida=Pro.stock-cant;
        }
        fclose(Arch);
    }

    return cant;

}

void Mostrar_Archivo_Pedidos() //Muestra todo el archivo de pedidos
{
    Pedido Pe;
    FILE *Arch=fopen("pedidos","rb");
    if(Arch!=NULL)
    {
        while(fread(&Pe,sizeof(Pedido),1,Arch)>0)
        {

            Mostrar_Pedido(Pe);
        }
    }
}

void Mostrar_Pedido(Pedido Pe) //Muestra un pedido
{
    puts("------------------------------------");
    printf("Pedido numero: %d\n",Pe.idPedido);
    printf("Cliente: %s %s\n",Pe.Client.nombre, Pe.Client.apellido);
    printf("Descripcion: %d ",Pe.cantidad);
    puts(Pe.Descripcion);
    printf("Costo total: %.2f\n",Pe.Costo);
    printf("Fecha de realizacion: %s hs\n",Pe.Fecha);
    if(Pe.Estado==1)
    {
        printf("Estado: Activo\n");
    }
    else
    {
        printf("Estado: Cancelado\n");
    }
    puts("------------------------------------");
}

void Fecha(char fechayhora[30]) //funcion que asigna una fecha y hora
{
    time_t t;
    struct tm *tm;


    t=time(NULL);
    tm=localtime(&t);
    strftime(fechayhora, 100, "%c", tm);


}

void Mostrar_Pedidos_Personales(cliente C) //Muestra los pedidos hechos por una persona
{
    FILE *Arch=fopen("pedidos","rb");
    Pedido aux;
    if(Arch!=NULL)
    {
        while(fread(&aux,sizeof(Pedido),1,Arch)>0) //Recorro todo el archivo de pedidos
        {
            if (C.idCliente==aux.Client.idCliente) //Si coinciden los Id muestro el pedido
            {
                Mostrar_Pedido(aux);

            }
        }
        printf("Su saldo total es de: $%.2f",Saldo_Cliente(C));
        fclose(Arch);

    }
}

void Dar_Baja_Pedido(cliente C) //Da de baja un pedido
{
    int Npe;
    Pedido Peaux;
    producto Proaux;
    FILE *ArchPedidos=fopen("pedidos","r+b");  // Abro el archivo de pedidos
    FILE *ArchProductos=fopen("stock","r+b");  // Abro el archivo de stock
    if(ArchPedidos!=NULL && ArchProductos!=NULL)
    {
        Mostrar_Pedidos_Personales(C); //Muestra solo los pedidos que hizo el usuario
        printf("\nIngrese el numero de pedido que quiere dar de baja: \n");
        scanf("%d",&Npe); //Elijo uno de esos pedidos
        fseek(ArchPedidos,(Npe-1)*sizeof(Pedido),SEEK_SET); //Me muevo hasta la posicion anterior
        fread(&Peaux,sizeof(Pedido),1,ArchPedidos); // Leo el pedido
        if(Peaux.Client.idCliente==C.idCliente) //si el Id del cliente cargado en el pedido es igual al del cliente
        {
            printf("Esta seguro de que desea realizar la operacion? S/N ");
            char op;
            fflush(stdin);
            scanf("%c",&op); //Si el usuario esta seguro que lo quiere dar de baja
            if(op=='s')
            {
                int pos=Buscar_Pos_Producto(Peaux.Product.IdProducto); //Busca la posicion del producto
                fseek(ArchProductos,(pos-1)*sizeof(producto),SEEK_SET); //Voy hasta una posicion anterior
                fread(&Proaux,sizeof(producto),1,ArchProductos); //Leo el producto
                Proaux.stock+=Peaux.cantidad; //Le agrego la cantidad que se habia pedido
                fseek(ArchProductos,(pos-1)*sizeof(producto),SEEK_SET); //Vuelvo a la posicion anterior
                fwrite(&Proaux,sizeof(producto),1,ArchProductos);// Lo escribo
                fseek(ArchPedidos,(Npe-1)*sizeof(Pedido),SEEK_SET); // Voy a la posicion anterior al pedido
                Peaux.Estado=0; // Le asigno al estado del pedido 0
                fwrite(&Peaux,sizeof(Pedido),1,ArchPedidos); // Lo escribo
            }
        }
        fclose(ArchPedidos);
        fclose(ArchProductos);  //Cierro ambos archivos
    }

}

void Mostar_Pedidos_Activos()// Muestra solo los pedidos que esten activos
{
    Pedido Pe;
    FILE *Arch=fopen("pedidos","rb");
    if(Arch!=NULL)
    {
        while (fread(&Pe,sizeof(Pedido),1,Arch))
        {
            if(Pe.Estado==1)  // Si el estado del pedido es 1 lo muestra
            {
                Mostrar_Pedido(Pe);
            }
        }
        fclose(Arch);
    }
}

void Mostar_Pedidos_Cancelados() //Muestra los pedidos dados de baja
{
    Pedido Pe;
    FILE *Arch=fopen("pedidos","rb");
    if(Arch!=NULL)
    {
        while (fread(&Pe,sizeof(Pedido),1,Arch))
        {
            if(Pe.Estado==0) //Si el estado del pedido es 0 lo muestra
            {
                Mostrar_Pedido(Pe);
            }
        }
        fclose(Arch);
    }
}

int Contar_Cantidad_Pedidos() // cuenta y retorna la cantidad de productos cargados
{
    FILE *Arch=fopen("pedidos","rb");
    Pedido p;
    int i=0;
    if(Arch!=NULL)
    {
        while(fread(&p,sizeof(Pedido),1,Arch)>0)
        {
            i++;
        }
        fclose(Arch);
    }
    return i;
}

float Saldo_Cliente(cliente c)
{
    FILE *ArchPedido=fopen("pedidos","rb");
    Pedido aux;
    float Suma=0;
    if(ArchPedido!=NULL)
    {
        while(fread(&aux,sizeof(Pedido),1,ArchPedido)>0)
        {
            if(c.idCliente==aux.Client.idCliente)
            {
                Suma+=aux.Costo;
            }
        }
        fclose(ArchPedido);
    }
    return Suma;
}


void Firma_Digital()
{
    int x=80;
    color(03);
    gotoxy(x,52);
    printf("Autores\n");
    gotoxy(x,53);
    printf("Barilatti Franco\n");
    gotoxy(x,54);
    printf("Barilatti Guido\n");
    color(00);
}
