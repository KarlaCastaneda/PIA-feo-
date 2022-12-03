#include <windows.h>
#include "resource.h"
#include <fstream>
#include <CommDlg.h>

using namespace std;

struct Usuario
{
	char usuarios_nombre[15];
	char usuarios_nombrecompleto[50];
	char usuarios_contrasenia[10];
	char usuarios_tipos[20];
	char usuarios_comercio[15];
	char usuarios_foto[300];
	char usuarios_direccion[100];
	char usuarios_primeregistro[5];
	Usuario* Usuario_sig;
	Usuario* Usuario_ant;
};
Usuario* Usuario_aux, * Usuario_inicio, * NUsuario, * miInfo = nullptr;
char zFile[MAX_PATH];
char direccion[MAX_PATH] = { 0 };

struct Cliente
{
	char Cliente_nombre[50];
	char Cliente_email[25];
	char Cliente_comercio[15];
	char Cliente_fecha[50];
	Cliente* Cliente_sig;
	Cliente* Cliente_ant;

};
Cliente* Cliente_inicio, * Cliente_aux = nullptr;

struct Consumo
{
	char Consumo_cliente[50];
	char Consumo_monto[50];
	char Consumo_subtotal[50];
	char Consumo_descuento[50];
	char Consumo_total[50];
	char Consumo_comercio[15];
	char Consumo_fecha[50];
	char Consumo_ID[50];
	Consumo* Consumo_sig;
	Consumo* Consumo_ant;
};
Consumo* Consumo_inicio, * Consumo_aux = nullptr;

struct Promocion
{
	char Promocion_nombre[50];
	char Promocion_monto[50];
	char Promocion_descuento[50];
	char Promocion_estatus[15];
	char Promocion_comercio[15];
	char Promocion_fecha[50];
	Promocion* Promocion_sig;
	Promocion* Promocion_ant;
};
Promocion* Promocion_inicio, * Promocion_aux = nullptr;

HMENU hmenu;
HINSTANCE hintance;
HWND ghDlg = 0;

void Agregar_usuario(Usuario* nuevo_u);
void Modificar_Usuario(Usuario* newInfo, char nomUsuario[50]);
void Eliminar_Usuario(char nomUsuario[50]);
void Escribir_Binario_Usuario();
void Leer_Binario_Usuario();

void Agregar_Cliente(Cliente* nuevo);
void Escribir_Binario_Cliente();
void Leer_Binario_Cliente();
void Eliminar_Cliente(char nomCliente[50]);

void Agregar_Consumo(Consumo* nuevo);
void Escribir_Binario_Consumo();
void Leer_Binario_Consumo();

void Agregar_Promocion(Promocion* nuevo);
void Escribir_Binario_Promocion();
void Leer_Binario_Promocion();
void Eliminar_Promocion(char nomPromocion[50]);


HINSTANCE hInstGlobal;
HWND hPrinGlob;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Registrar(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK VentanaInicio(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK InfoUsuario(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ListaUsuarios(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK AltaUsuario(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK RestablecerUS(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK AltaCliente(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ListaClientes(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK AltaConsumos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ListaConsumos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FiltroConsumos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK AltaPromocion(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ListaPromocion(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int cmdshow)
{
	Leer_Binario_Usuario();
	Leer_Binario_Cliente();
	Leer_Binario_Consumo();
	Leer_Binario_Promocion();
    hInstGlobal = hInst;
    HWND hVInicio = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), 0, (DLGPROC)WindowProc);
    ShowWindow(hVInicio, cmdshow);

    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));
    while (GetMessage(&msg, 0, 0, 0))
    {
        if (hInst == 0 || !IsDialogMessage(hPrinGlob, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
	Escribir_Binario_Usuario();
	Escribir_Binario_Cliente();
	Escribir_Binario_Consumo();
    return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {

    case WM_INITDIALOG:
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
		case 1012: { //Validacion de inicio de sesion
			char usuarios_nombre[15];
			char usuarios_contrasenia[10];
			char usuarios_comercio[15];
			GetDlgItemText(hwnd, 1001, usuarios_nombre, sizeof(usuarios_nombre));
			GetDlgItemText(hwnd, 1002, usuarios_contrasenia, sizeof(usuarios_contrasenia));
			GetDlgItemText(hwnd, 1003, usuarios_comercio, sizeof(usuarios_comercio));

			Usuario_aux = Usuario_inicio;
			if (Usuario_inicio == nullptr) {
				MessageBox(hwnd, "No hay usuario registrados.", "AVISO", MB_OK | MB_ICONERROR);
			}
			else {
				while (Usuario_aux != nullptr && strcmp(Usuario_aux->usuarios_nombre, usuarios_nombre) != 0) {
					Usuario_aux = Usuario_aux->Usuario_sig;
				}
				if (Usuario_aux == nullptr) {
					MessageBox(hwnd, "Usuario no encontrado", "AVISO", MB_OK | MB_ICONERROR);
				}
				else {
					if (strcmp(Usuario_aux->usuarios_contrasenia, usuarios_contrasenia) == 0)
					{
						if (strcmp(Usuario_aux->usuarios_comercio, usuarios_comercio) == 0) {
							NUsuario = Usuario_aux;
							miInfo = Usuario_aux;
							MessageBox(hwnd, "Ya puede ingresar", "AVISO", MB_OK | MB_ICONEXCLAMATION);

							if (strcmp(Usuario_aux->usuarios_primeregistro, "0") == 0) {//validacion de si ya ingreso antes
								EndDialog(hwnd, 0);
								DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG4), hwnd, InfoUsuario);
							}
							else {
								EndDialog(hwnd, 0);
								DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG3), hwnd, VentanaInicio);
							}
						}
						else {
							MessageBox(hwnd, "Comercio incorrecto", "AVISO", MB_OK | MB_ICONERROR);
						}
					}
					else {
						MessageBox(hwnd, "Contraseña incorrecta", "AVISO", MB_OK | MB_ICONERROR);
					}
				}
			}
		}
				 break;
        case 1005:
        {
            EndDialog(hwnd, 0);
            DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG2), hwnd, Registrar);

        }
        break;

		
		case 1013: {
			Escribir_Binario_Usuario();
			DestroyWindow(hwnd);
			PostQuitMessage(0);
		}
				 break;
		}
       
        break;


    }

    return FALSE;
}

LRESULT CALLBACK Registrar(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) { 
    switch (msg) {
        {

    case WM_INITDIALOG:
	{
		char TipodeUsuario[2][14] = { "Administrador", "Cajero" };
		for (int i = 0; i < 2; i++) {
			SendDlgItemMessage(hwnd, 1011, CB_INSERTSTRING, (WPARAM)i, (LPARAM)TipodeUsuario[i]);

		}
		SendMessage(GetDlgItem(hwnd, 1011), CB_SETCURSEL, (WPARAM)0, 0);
	}
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
		case  1007: {//De Registro REGRESAR a Inicio Sesion
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG1), NULL, WindowProc);
		}
				  break;

		case 1006: { //Boton de Aceptar registro
			int CLetras = 0;
			int CNumeros = 0;
			int CCaracteres = 0;
			int CEspacios = 0;
			int lengthNombre = 0;
			int lengthComercio = 0;
			int lengthContrasena = 0;

			Usuario* Nuevo_usuario = new Usuario;
			GetDlgItemText(hwnd, 1008, Nuevo_usuario->usuarios_nombre, sizeof(Nuevo_usuario->usuarios_nombre));
			GetDlgItemText(hwnd, 1009, Nuevo_usuario->usuarios_contrasenia, sizeof(Nuevo_usuario->usuarios_contrasenia));
			GetDlgItemText(hwnd, 1010, Nuevo_usuario->usuarios_comercio, sizeof(Nuevo_usuario->usuarios_comercio));

			int UsuarioSelec = 0;
			UsuarioSelec = SendMessage(GetDlgItem(hwnd, 1011), CB_GETCURSEL, 0, 0);
			GetDlgItemText(hwnd, 1011, Nuevo_usuario->usuarios_tipos, sizeof(Nuevo_usuario->usuarios_tipos));
			//Nuevo_usuario->usuarios_tipos = UsuarioSelec + 1;
			lengthComercio = strlen(Nuevo_usuario->usuarios_comercio);
			lengthNombre = strlen(Nuevo_usuario->usuarios_nombre);
			lengthContrasena = strlen(Nuevo_usuario->usuarios_contrasenia);

			char usuarios_nombre_registro[15];
			GetDlgItemText(hwnd, 1008, usuarios_nombre_registro, sizeof(usuarios_nombre_registro));
			Usuario_aux = Usuario_inicio;



			//VALIDACION
			if (lengthNombre <= 0 || lengthNombre > 15)//valida nombre
			{
				MessageBox(hwnd, "Favor de ingresar de un maximo de 15 caracteres alfabeticos (Sin espacios).", "NOMBRE", MB_OK | MB_ICONEXCLAMATION);
			}
			else {
				for (int i = 0; i < lengthNombre; i++)
				{
					if (ispunct(Nuevo_usuario->usuarios_nombre[i]))
					{
						CCaracteres++;
					}
					if (isdigit(Nuevo_usuario->usuarios_nombre[i]))
					{
						CNumeros++;
					}
					if (isspace(Nuevo_usuario->usuarios_nombre[i]))
					{
						CEspacios++;
					}
				}
				if (CCaracteres != 0 || CNumeros != 0 || CEspacios != 0)
				{
					MessageBox(hwnd, "Favor de solo ingresar caracteres alfabeticos (Sin espacios).", "NOMBRE", MB_OK | MB_ICONEXCLAMATION);
				}
				else
				{
					CLetras = 0;
					CNumeros = 0;
					CCaracteres = 0;
					CEspacios = 0;
					while (Usuario_aux != nullptr && strcmp(Usuario_aux->usuarios_nombre, usuarios_nombre_registro) != 0) {//validacion mismmo nombre
						Usuario_aux = Usuario_aux->Usuario_sig;
					}
					if (Usuario_aux != nullptr) {
						MessageBox(hwnd, "Usuario ya existente, intente otro nombre", "AVISO", MB_OK | MB_ICONERROR);
					}
					else {
						if (lengthContrasena < 3 || lengthContrasena>10)//valida contraseña
						{
							MessageBox(hwnd, "Favor de ingresar de 3 a 10 caracteres. Minimo 1 letra, 1 numero y 1 caracter especial (Sin espacios).", "CONTRASEÑA", MB_OK | MB_ICONEXCLAMATION);
						}
						else {
							for (int i = 0; i < lengthContrasena; i++)
							{
								if (isalpha(Nuevo_usuario->usuarios_contrasenia[i]))
								{
									CLetras++;
								}
								if (ispunct(Nuevo_usuario->usuarios_contrasenia[i]))
								{
									CCaracteres++;
								}
								if (isdigit(Nuevo_usuario->usuarios_contrasenia[i]))
								{
									CNumeros++;
								}
								if (isspace(Nuevo_usuario->usuarios_contrasenia[i]))
								{
									CEspacios++;
								}
							}
							if (CCaracteres == 0 || CNumeros == 0 || CLetras == 0 || CEspacios != 0)
							{
								MessageBox(hwnd, "Favor de ingresar minimo 1 letra, 1 numero y 1 caracter especial(Sin espacios).", "CONTRASEÑA", MB_OK | MB_ICONEXCLAMATION);
							}
							else
							{
								CLetras = 0;
								CNumeros = 0;
								CCaracteres = 0;
								CEspacios = 0;
								if (lengthComercio < 5 || lengthComercio>15)//valida comercio
								{
									MessageBox(hwnd, "Favor de ingresar de 5 a 15 caracteres alfanumericos o espacios", "COMERCIO", MB_OK | MB_ICONEXCLAMATION);
								}
								else
								{
									for (int i = 0; i < lengthComercio; i++)
									{
										if (ispunct(Nuevo_usuario->usuarios_comercio[i]))
										{
											CCaracteres++;
										}
									}
									if (CCaracteres != 0)
									{
										MessageBox(hwnd, "Favor de solo ingresar caracteres alfanumericos o espacios", "COMERCIO", MB_OK | MB_ICONEXCLAMATION);
									}
									else
									{//Registro exitoso
										//MessageBox(hwnd, "Usuario registrado", "AVISO", MB_OK | MB_ICONEXCLAMATION);
										strcpy_s(Nuevo_usuario->usuarios_foto, "");
										strcpy_s(Nuevo_usuario->usuarios_tipos, "Administrador");
										strcpy_s(Nuevo_usuario->usuarios_nombrecompleto, "");
										strcpy_s(Nuevo_usuario->usuarios_direccion, "");
										strcpy_s(Nuevo_usuario->usuarios_primeregistro, "0");
										Agregar_usuario(Nuevo_usuario);
										delete Nuevo_usuario;
										Escribir_Binario_Usuario();

										SetDlgItemText(hwnd, 1008, "");
										SetDlgItemText(hwnd, 1009, "");
										SetDlgItemText(hwnd, 1010, "");
									}
								}
							}
						}
					}

				}
			}

		}break;
		}
		}
		break;
	}
	
return false;
}
LRESULT CALLBACK VentanaInicio(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {


	case WM_INITDIALOG:
		if (strcmp(NUsuario->usuarios_tipos, "Cajero") == 0)
		{
			EnableMenuItem(GetMenu(hwnd), 40003, MF_DISABLED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hwnd), 40004, MF_DISABLED | MF_BYCOMMAND);
			/*EnableMenuItem(GetMenu(hwnd), 993, MF_DISABLED | MF_BYCOMMAND);*/

		}
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 40001:
		{
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG4), hwnd, InfoUsuario);
		}
		break;
		case 40002: {
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG5), hwnd, ListaUsuarios);
		}
				  break;
		case 40003: {
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG6), hwnd, AltaUsuario);
		}
				  break;
		case 40013: {
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, WindowProc);
		}
				  break;
		case 40004: {
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG7), hwnd, RestablecerUS);
		}
				  break;
		case 40005: {
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG8), hwnd, AltaCliente);

		}
				  break;

		case 40006: {
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG9), hwnd, ListaClientes);

		}
				  break;
		case  40007: {
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG10), hwnd, AltaConsumos);
		}
				   break;
		case 40008: {
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG11), hwnd, ListaConsumos);
		}
				  break;
		case 40009: {
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG12), hwnd, FiltroConsumos);
		}
				  break;
		case 40010: {
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG13), hwnd, AltaPromocion);
		} break;
			case 40011: {
				EndDialog(hwnd, 0);
				DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG14), hwnd, ListaPromocion);
			}
					  break;

					  /*}*/


		}
		
	}
	return FALSE;
}

LRESULT CALLBACK InfoUsuario(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
		{

	case WM_INITDIALOG:
	{
		SetDlgItemText(hwnd, 1014, NUsuario->usuarios_nombre);
		SetDlgItemText(hwnd, 1015, NUsuario->usuarios_nombrecompleto);
		SetDlgItemText(hwnd, 1020, NUsuario->usuarios_comercio);
		SetDlgItemText(hwnd, 1016, NUsuario->usuarios_direccion);
		strcpy_s(zFile, NUsuario->usuarios_foto);


		SendDlgItemMessage(hwnd, 1019, CB_SELECTSTRING, 0, (LPARAM)NUsuario->usuarios_tipos);

		/*SendDlgItemMessage(hwnd, 1022, EM_LIMITTEXT, 50, 5);*/

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, NUsuario->usuarios_foto, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
		SendDlgItemMessage(hwnd, 1022, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

		char TipodeUsuario[2][14] = { "Administrador", "Cajero" };
		for (int i = 0; i < 2; i++) {
			SendDlgItemMessage(hwnd, 1019, CB_INSERTSTRING, (WPARAM)i, (LPARAM)TipodeUsuario[i]);
		}
		SendMessage(GetDlgItem(hwnd, 1019), CB_SELECTSTRING, 0, (LPARAM)NUsuario->usuarios_tipos);
		/*SendMessage(GetDlgItem(hwnd, 1024), CB_SETCURSEL, (WPARAM)NUsuario->usuarios_tipos - 1, 0);*/

	}
	break;


	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 1018: {
			EnableWindow(GetDlgItem(hwnd, 1014), true);
			EnableWindow(GetDlgItem(hwnd, 1015), true);
			EnableWindow(GetDlgItem(hwnd, 1020), true);
			EnableWindow(GetDlgItem(hwnd, 1005), true);
		}
			  break;
		case 1021:
		{//Registrar aceptar cambios
			int CLetras = 0;
			int CNumeros = 0;
			int CCaracteres = 0;
			int CEspacios = 0;
			int lengthNombre = 0;
			int lengthComercio = 0;
			int lengthContrasena = 0;
			GetDlgItemText(hwnd, 1015, NUsuario->usuarios_nombrecompleto, sizeof(NUsuario->usuarios_nombrecompleto));
			GetDlgItemText(hwnd, 1016, NUsuario->usuarios_direccion, sizeof(NUsuario->usuarios_direccion));


			lengthNombre = strlen(NUsuario->usuarios_nombrecompleto);

			int UsuarioSelec = 0;
			UsuarioSelec = SendMessage(GetDlgItem(hwnd, 1022), CB_GETCURSEL, 0, 0);
			strcpy_s(NUsuario->usuarios_foto, zFile);
			if (strcmp(NUsuario->usuarios_nombrecompleto, "") == 0) {//validacion si ya escribio nombre
				MessageBox(hwnd, "Favor de escribir su nombre completo", "AVISO", MB_OK | MB_ICONERROR);
				strcpy_s(NUsuario->usuarios_primeregistro, "0");

			}
			else {
				if (lengthNombre < 5 || lengthNombre > 50)
				{
					MessageBox(hwnd, "Favor de ingresar de 5 a 50 caracteres alfabeticos.", "NOMBRE", MB_OK | MB_ICONEXCLAMATION);
					strcpy_s(NUsuario->usuarios_primeregistro, "0");

				}
				else {
					for (int i = 0; i < lengthNombre; i++)
					{
						if (ispunct(NUsuario->usuarios_nombrecompleto[i]))
						{
							CCaracteres++;
						}
						if (isdigit(NUsuario->usuarios_nombrecompleto[i]))
						{
							CNumeros++;
						}
					}
					if (CCaracteres != 0 || CNumeros != 0)
					{
						MessageBox(hwnd, "Favor de solo ingresar caracteres alfabeticos.", "NOMBRE", MB_OK | MB_ICONEXCLAMATION);
						strcpy_s(NUsuario->usuarios_primeregistro, "0");
					}
					else
					{
						if (strcmp(NUsuario->usuarios_direccion, "") == 0) {//validacion si ya eligio foto
							MessageBox(hwnd, "Favor de seleccionar una foto", "AVISO", MB_OK | MB_ICONERROR);
							strcpy_s(NUsuario->usuarios_primeregistro, "0");
						}
						else {
							strcpy_s(NUsuario->usuarios_primeregistro, "1");
							EnableWindow(GetDlgItem(hwnd, 1014), false);
							EnableWindow(GetDlgItem(hwnd, 1015), false);
							EnableWindow(GetDlgItem(hwnd, 1020), false);
							EnableWindow(GetDlgItem(hwnd, 1005), false);
						}
					}
				}
			}
		}
		break;
		case 1005: {
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "ALL\0 * .*\0Bitmaps\0 * .bmp\0";
			ofn.lpstrFile = zFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(zFile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 2;

			if (GetOpenFileName(&ofn) == TRUE) {
				HBITMAP bmp; //1

				bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE); //2
				SetDlgItemText(hwnd, 1016, zFile);
				SendDlgItemMessage(hwnd, 1022, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp); //3 
			}
			else {
				MessageBox(hwnd, "No seleccionó ninguna foto", "AVISO", MB_OK | MB_ICONERROR);
				strcpy_s(zFile, NUsuario->usuarios_foto);

			}
		}break;
		case 1017: {//Regresar a menu
			if (strcmp(NUsuario->usuarios_primeregistro, "0") == 0) {//validacion de si ya ingreso antes
				MessageBox(hwnd, "Favor de registrar sus datos", "AVISO", MB_OK | MB_ICONERROR);
			}
			else {
				Escribir_Binario_Usuario();
				EndDialog(hwnd, 0);
				DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG3), NULL, VentanaInicio);
			}
		}
				 break;

				 break;
		}
		

		}
	}return false;
}
LRESULT CALLBACK ListaUsuarios(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	char textoList[50] = { 0 };
	switch (msg) {
		

	case WM_INITDIALOG:
	{//rellena lista de usuarios

		char Usuario_comercio_c[15];
		strcpy_s(Usuario_comercio_c, NUsuario->usuarios_comercio);

		char TipodeUsuario[2][14] = { "Administrador", "Cajero" };
		for (int i = 0; i < 2; i++) {
			SendDlgItemMessage(hwnd, 1033, CB_INSERTSTRING, (WPARAM)i, (LPARAM)TipodeUsuario[i]);
		}


		Usuario_aux = Usuario_inicio;
		while (Usuario_aux != nullptr) {
			if (strcmp(Usuario_comercio_c, Usuario_aux->usuarios_comercio) == 0) {
				SendMessage(GetDlgItem(hwnd, 1023), LB_ADDSTRING, 0, (LPARAM)Usuario_aux->usuarios_nombre);
			}
			Usuario_aux = Usuario_aux->Usuario_sig;
		}

		if (strcmp(NUsuario->usuarios_tipos, "Cajero") == 0)
		{
			EnableWindow(GetDlgItem(hwnd, 1024), false);
			EnableWindow(GetDlgItem(hwnd, 1025), false);
			EnableWindow(GetDlgItem(hwnd, 1030), false);

		}
		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, NUsuario->usuarios_foto, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
		SendDlgItemMessage(hwnd, 1035, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		SetDlgItemText(hwnd, 1034, NUsuario->usuarios_nombre);
	}
	break;
		

	case WM_COMMAND:
	
		if (LOWORD(wParam) == 1023) {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {

				int indice = 0;

				indice = SendDlgItemMessage(hwnd, 1023, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, 1023, LB_GETTEXT, indice, (LPARAM)textoList);

				Usuario_aux = Usuario_inicio;
				while (Usuario_aux != nullptr && strcmp(textoList, Usuario_aux->usuarios_nombre) != 0) {
					Usuario_aux = Usuario_aux->Usuario_sig;
				}
				if (Usuario_aux == nullptr) {

				}
				else
				{
					SetDlgItemText(hwnd, 1024, Usuario_aux->usuarios_nombre);
					SetDlgItemText(hwnd, 1025, Usuario_aux->usuarios_nombrecompleto);
					SetDlgItemText(hwnd, 1026, Usuario_aux->usuarios_comercio);
					SetDlgItemText(hwnd, 1032, Usuario_aux->usuarios_direccion);
					SendDlgItemMessage(hwnd, 1033, CB_SELECTSTRING, 0, (LPARAM)Usuario_aux->usuarios_tipos);
					HBITMAP bmp;
					bmp = (HBITMAP)LoadImage(NULL, Usuario_aux->usuarios_foto, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
					SendDlgItemMessage(hwnd, 1036, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
				}

			}break;
			}
		}
	

	if (LOWORD(wParam) == 1029) {//Registrar
		int indice = 0;
		int CLetras = 0;
		int CNumeros = 0;
		int CCaracteres = 0;
		int CEspacios = 0;
		int lengthNombre = 0;

		char Nnombre[51] = { 0 };
		char Ndireccion[100] = { 0 };
		char Nfoto[300] = { 0 };
		char Ntipo[20] = { 0 };
		//Usuario* newInfoUser = new Usuario;
		Usuario_aux = Usuario_inicio;
		indice = SendDlgItemMessage(hwnd, 1023, LB_GETCURSEL, 0, 0);
		SendDlgItemMessage(hwnd, 1023, LB_GETTEXT, indice, (LPARAM)textoList);


		GetDlgItemText(hwnd, 1025, Nnombre, sizeof(Nnombre));
		GetDlgItemText(hwnd, 1033, Ntipo, sizeof(Ntipo));
		GetDlgItemText(hwnd, 1036, Nfoto, sizeof(Nfoto));
		GetDlgItemText(hwnd, 1032, Ndireccion, sizeof(Ndireccion));

		//Modificar_Usuario(newInfoUser, textoList);

		while (Usuario_aux != nullptr && strcmp(Usuario_aux->usuarios_nombre, textoList) != 0) {

			Usuario_aux = Usuario_aux->Usuario_sig;

		}
		if (Usuario_aux == nullptr) {
			MessageBeep(MB_OK);
			MessageBox(hwnd, "No se encontro usuario", "ADVERTENCIA", MB_OK | MB_ICONERROR);

		}
		else
		{
			lengthNombre = strlen(Nnombre);
			if (lengthNombre < 5 || lengthNombre > 50)
			{
				MessageBox(hwnd, "Favor de ingresar de 5 a 50 caracteres alfabeticos.", "NOMBRE", MB_OK | MB_ICONEXCLAMATION);
				/*	strcpy_s(Usuario_aux->Usuario_pv, "0");*/
			}
			else {
				for (int i = 0; i < lengthNombre; i++)
				{
					if (ispunct(Nnombre[i]))
					{
						CCaracteres++;
					}
					if (isdigit(Nnombre[i]))
					{
						CNumeros++;
					}
				}
				if (CCaracteres != 0 || CNumeros != 0)
				{
					MessageBox(hwnd, "Favor de solo ingresar caracteres alfabeticos.", "NOMBRE", MB_OK | MB_ICONEXCLAMATION);
					/*strcpy_s(NUsuario->Usuario_pv, "0");*/
				}
				else
				{
					int Opcion = MessageBox(0, "¿Seguro desea modificar el usuario?", "AVISO", MB_YESNO | MB_ICONQUESTION);
					switch (Opcion) {
					case IDYES: {
						if (strcmp(Nnombre, "") != 0 && strcmp(Ndireccion, "") != 0)
						{
							strcpy_s(Usuario_aux->usuarios_primeregistro, "1");
						}
						strcpy_s(Usuario_aux->usuarios_nombrecompleto, Nnombre);
						strcpy_s(Usuario_aux->usuarios_tipos, Ntipo);
						strcpy_s(Usuario_aux->usuarios_foto, Ndireccion);
						strcpy_s(Usuario_aux->usuarios_direccion, Ndireccion);

						MessageBox(0, "Usuario modificado", "AVISO", MB_OK | MB_ICONINFORMATION);
						EndDialog(hwnd, 0);
						Escribir_Binario_Usuario();
						DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG5), hwnd, ListaUsuarios);
					}
							  break;
					}

				}

			}
		}
	}
	if (LOWORD(wParam) == 1027) {//Eliminar
		int indice = 0;

		indice = SendDlgItemMessage(hwnd, 1023, LB_GETCURSEL, 0, 0);
		SendDlgItemMessage(hwnd, 1023, LB_GETTEXT, indice, (LPARAM)textoList);

		if (strcmp(textoList, NUsuario->usuarios_nombre) == 0)
		{
			MessageBox(hwnd, "No se puede eliminar un usuario activo, favor de cerrar sesión y usar otro usuario", "AVISO", MB_OK | MB_ICONERROR);
		}
		else {
			int Opcion = MessageBox(0, "¿Seguro que desea eliminar al usuario?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			if (Opcion == IDYES) {
				Eliminar_Usuario(textoList);

				SendMessage(GetDlgItem(hwnd, 1023), LB_DELETESTRING, (WPARAM)indice, 0);
				SendMessage(GetDlgItem(hPrinGlob, 1023), LB_RESETCONTENT, 0, 0);
				Escribir_Binario_Usuario();

				Usuario_aux = Usuario_inicio;
				while (Usuario_aux != nullptr) {
					SendMessage(GetDlgItem(hPrinGlob, 1023), LB_ADDSTRING, 0, (LPARAM)Usuario_aux->usuarios_nombre);
					Usuario_aux = Usuario_aux->Usuario_sig;
				}
			}

		}

	}
	if (LOWORD(wParam) == 1030) {//modificar
		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(ofn));

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hwnd;
		ofn.lpstrFilter = "ALL\0 * .*\0Bitmaps\0 * .bmp\0";
		ofn.lpstrFile = zFile;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(zFile);
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		ofn.nFilterIndex = 2;

		if (GetOpenFileName(&ofn) == TRUE) {
			HBITMAP bmp; //1

			bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE); //2
			SetDlgItemText(hwnd, 1032, zFile);
			SendDlgItemMessage(hwnd, 1036, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp); //3 
		}
		else {
			MessageBox(hwnd, "No seleccionó foto", "AVISO", MB_OK | MB_ICONERROR);
			strcpy_s(zFile, NUsuario->usuarios_foto);

		}
	}
	if (LOWORD(wParam) == 1031) {//modificar
		if (strcmp(NUsuario->usuarios_tipos, "Administrador") == 0)
		{
			EnableWindow(GetDlgItem(hwnd, 1030), true);
			EnableWindow(GetDlgItem(hwnd, 1025), true);
			EnableWindow(GetDlgItem(hwnd, 1033), true);
		}
	}
	if (LOWORD(wParam) == 1028) { 	
		int Opcion = MessageBox(0, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (Opcion) {
		case IDYES: {
			EndDialog(hwnd,0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG3), NULL, VentanaInicio);
		}
				  break;
		}
	}
	} return FALSE;
	
	}
LRESULT CALLBACK AltaUsuario(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: { //COMBO BOX PARA TIPO DE USUARIO

		SetDlgItemText(hwnd, 1042, NUsuario->usuarios_comercio);
		SendDlgItemMessage(hwnd, 1043, CB_SELECTSTRING, 0, (LPARAM)NUsuario->usuarios_tipos);

		char TipodeUsuario[2][14] = { "Administrador", "Cajero" };
		for (int i = 0; i < 2; i++) {
			SendDlgItemMessage(hwnd, 1043, CB_INSERTSTRING, (WPARAM)i, (LPARAM)TipodeUsuario[i]);
		}
		SendMessage(GetDlgItem(hwnd, 1043), CB_SELECTSTRING, 0, (LPARAM)NUsuario->usuarios_tipos);

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, NUsuario->usuarios_foto, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
		SendDlgItemMessage(hwnd, 1044, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		SetDlgItemText(hwnd, 1039, NUsuario->usuarios_nombre);
		/*SendMessage(GetDlgItem(hwnd, 1043), CB_SETCURSEL, (WPARAM)0, 0);*/
	}break;

	case WM_COMMAND: {
		switch (LOWORD(wParam)) {

		case 1041: {//REGRESAR A LA VENTANA PRINCIPAL
			Escribir_Binario_Usuario();
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG3), NULL, VentanaInicio);
		}
				 break;
		case 1040: { //BOTON ACEPTAR EL ALTA DEL USUARIO 
			int CLetras = 0;
			int CNumeros = 0;
			int CCaracteres = 0;
			int CEspacios = 0;
			int lengthNombre = 0;
			int lengthComercio = 0;
			int lengthContrasena = 0;


			Usuario* Nuevo_usuario = new Usuario;
			GetDlgItemText(hwnd, 1037, Nuevo_usuario->usuarios_nombre, sizeof(Nuevo_usuario->usuarios_nombre));
			GetDlgItemText(hwnd, 1038, Nuevo_usuario->usuarios_contrasenia, sizeof(Nuevo_usuario->usuarios_contrasenia));
			GetDlgItemText(hwnd, 1042, Nuevo_usuario->usuarios_comercio, sizeof(Nuevo_usuario->usuarios_comercio));
			//REVISAR EL TIPO DE USUARIO					 
			int UsuarioSelec = 0;
			UsuarioSelec = SendMessage(GetDlgItem(hwnd, 1043), CB_GETCURSEL, 0, 0);
			/*GetDlgItemText(hwnd, 1024, Nuevo_usuario->Usuario_tipo, sizeof(Nuevo_usuario->Usuario_tipo));*/
			GetDlgItemText(hwnd, 1043, Nuevo_usuario->usuarios_tipos, sizeof(Nuevo_usuario->usuarios_tipos));
			/*Nuevo_usuario->Usuario_tipo = UsuarioSelec + 1;*/
			lengthNombre = strlen(Nuevo_usuario->usuarios_nombre);
			lengthContrasena = strlen(Nuevo_usuario->usuarios_contrasenia);
			char Usuario_nombre_registro[15];
			GetDlgItemText(hwnd, 1037, Usuario_nombre_registro, sizeof(Usuario_nombre_registro));
			Usuario_aux = Usuario_inicio;

			if (lengthNombre <= 0 || lengthNombre > 15)//valida nombre
			{
				MessageBox(hwnd, "Favor de ingresar de un maximo de 15 caracteres alfabeticos (Sin espacios).", "NOMBRE", MB_OK | MB_ICONEXCLAMATION);
			}
			else {
				for (int i = 0; i < lengthNombre; i++)
				{
					if (ispunct(Nuevo_usuario->usuarios_nombre[i]))
					{						   
						CCaracteres++;		   
					}						   
					if (isdigit(Nuevo_usuario->usuarios_nombre[i]))
					{						   
						CNumeros++;			   
					}						   
					if (isspace(Nuevo_usuario->usuarios_nombre[i]))
					{
						CEspacios++;
					}
				}
				if (CCaracteres != 0 || CNumeros != 0 || CEspacios != 0)
				{
					MessageBox(hwnd, "Favor de solo ingresar caracteres alfabeticos (Sin espacios).", "NOMBRE", MB_OK | MB_ICONEXCLAMATION);
				}
				else
				{
					CLetras = 0;
					CNumeros = 0;
					CCaracteres = 0;
					CEspacios = 0;
					while (Usuario_aux != nullptr && strcmp(Usuario_aux->usuarios_nombre, Usuario_nombre_registro) != 0) {//validacion mismmo nombre
						Usuario_aux = Usuario_aux->Usuario_sig;
					}
					if (Usuario_aux != nullptr) {
						MessageBox(hwnd, "Usuario ya existente, intente otro nombre", "AVISO", MB_OK | MB_ICONERROR);
					}
					else {
						if (lengthContrasena < 3 || lengthContrasena>10)//valida contraseña
						{
							MessageBox(hwnd, "Favor de ingresar de 3 a 10 caracteres. Minimo 1 letra, 1 numero y 1 caracter especial (Sin espacios).", "CONTRASEÑA", MB_OK | MB_ICONEXCLAMATION);
						}
						else {
							for (int i = 0; i < lengthContrasena; i++)
							{
								if (isalpha(Nuevo_usuario->usuarios_contrasenia[i]))
								{
									CLetras++;
								}
								if (ispunct(Nuevo_usuario->usuarios_contrasenia[i]))
								{
									CCaracteres++;
								}
								if (isdigit(Nuevo_usuario->usuarios_contrasenia[i]))
								{
									CNumeros++;
								}
								if (isspace(Nuevo_usuario->usuarios_contrasenia[i]))
								{
									CEspacios++;
								}
							}
							if (CCaracteres == 0 || CNumeros == 0 || CLetras == 0 || CEspacios != 0)
							{
								MessageBox(hwnd, "Favor de ingresar minimo 1 letra, 1 numero y 1 caracter especial(Sin espacios).", "CONTRASEÑA", MB_OK | MB_ICONEXCLAMATION);
							}
							else
							{//registrro exitoso
								strcpy_s(Nuevo_usuario->usuarios_foto, "");
								strcpy_s(Nuevo_usuario->usuarios_nombrecompleto, "");
								strcpy_s(Nuevo_usuario->usuarios_direccion, "");
								strcpy_s(Nuevo_usuario->usuarios_primeregistro, "0");
								Agregar_usuario(Nuevo_usuario);
								delete Nuevo_usuario;
								Escribir_Binario_Usuario();

								SetDlgItemText(hwnd, 1037, "");
								SetDlgItemText(hwnd, 1038, "");
							}
						}
					}

				}
			}
		}break;
		
		}

	}
	}return FALSE;
}
LRESULT CALLBACK RestablecerUS(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	char textoList[50] = { 0 };
	switch (msg) {


	case WM_INITDIALOG:{
		char Usuario_comercio_c[15];
		strcpy_s(Usuario_comercio_c, NUsuario->usuarios_comercio);


		Usuario_aux = Usuario_inicio;
		while (Usuario_aux != nullptr) {
			if (strcmp(Usuario_comercio_c, Usuario_aux->usuarios_comercio) == 0)
			{
				SendMessage(GetDlgItem(hwnd, 1052), LB_ADDSTRING, 0, (LPARAM)Usuario_aux->usuarios_nombre);

			}
			Usuario_aux = Usuario_aux->Usuario_sig;
		}

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, NUsuario->usuarios_foto, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
		SendDlgItemMessage(hwnd, 1049, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		SetDlgItemText(hwnd, 1048, NUsuario->usuarios_nombre);
	}
		break;

	case WM_COMMAND:
		if (LOWORD(wParam) == 1052) {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {

				int indice = 0;

				indice = SendDlgItemMessage(hwnd, 1052, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, 1052, LB_GETTEXT, indice, (LPARAM)textoList);

				Usuario_aux = Usuario_inicio;
				while (Usuario_aux != nullptr && strcmp(textoList, Usuario_aux->usuarios_nombre) != 0) {
					Usuario_aux = Usuario_aux->Usuario_sig;
				}
				if (Usuario_aux == nullptr) {

				}
				else
				{
					SetDlgItemText(hwnd, 1046, Usuario_aux->usuarios_contrasenia);
				}

			}break;
			}
		}
		if (LOWORD(wParam) == 1050) {//MODIFICAR CONTRASEÑA
			int indice = 0;

			char NContra[50] = { 0 };
			//Usuario* newInfoUser = new Usuario;
			Usuario_aux = Usuario_inicio;
			indice = SendDlgItemMessage(hwnd, 1052, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, 1052, LB_GETTEXT, indice, (LPARAM)textoList);


			GetDlgItemText(hwnd, 1047, NContra, sizeof(NContra));
			int CLetras = 0;
			int CNumeros = 0;
			int CCaracteres = 0;
			int CEspacios = 0;
			int lengthContrasena = 0;

			lengthContrasena = strlen(NContra);
			//Modificar_Usuario(newInfoUser, textoList);
			while (Usuario_aux != nullptr && strcmp(Usuario_aux->usuarios_nombre, textoList) != 0) {

				Usuario_aux = Usuario_aux->Usuario_sig;

			}
			if (Usuario_aux == nullptr) {
				MessageBeep(MB_OK);
				MessageBox(hwnd, "No hay usuario seleccionado", "ADVERTENCIA", MB_OK | MB_ICONERROR);
			}
			else
			{
				if (lengthContrasena < 3 || lengthContrasena>10)
				{
					MessageBox(hwnd, "Favor de ingresar de 3 a 10 caracteres. Minimo 1 letra, 1 numero y 1 caracter especial (Sin espacios).", "CONTRASEÑA", MB_OK | MB_ICONEXCLAMATION);
				}
				else {
					for (int i = 0; i < lengthContrasena; i++)
					{
						if (isalpha(NContra[i]))
						{
							CLetras++;
						}
						if (ispunct(NContra[i]))
						{
							CCaracteres++;
						}
						if (isdigit(NContra[i]))
						{
							CNumeros++;
						}
						if (isspace(NContra[i]))
						{
							CEspacios++;
						}
					}
					if (CCaracteres == 0 || CNumeros == 0 || CLetras == 0 || CEspacios != 0)
					{
						MessageBox(hwnd, "Favor de ingresar minimo 1 letra, 1 numero y 1 caracter especial(Sin espacios).", "CONTRASEÑA", MB_OK | MB_ICONEXCLAMATION);
					}
					else
					{
						strcpy_s(Usuario_aux->usuarios_contrasenia, NContra);
						MessageBox(0, "Contraseña modificadaa", "AVISO", MB_OK | MB_ICONINFORMATION);
						Escribir_Binario_Usuario();
						EndDialog(hwnd, 0);
						DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG7), hwnd, RestablecerUS);
					}
				}


			}
		}
		if (LOWORD(wParam) == 1051) {
			Escribir_Binario_Usuario();
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG3), NULL, VentanaInicio);
		}
	}
	return FALSE;
}
LRESULT CALLBACK AltaCliente(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{

	case WM_INITDIALOG: {
		SYSTEMTIME fechaHoy;
		ZeroMemory(&fechaHoy, sizeof(fechaHoy));
		GetLocalTime(&fechaHoy);

		char fechaCompleta[11] = { 0 };
		char charFecha[5] = { 0 };

		sprintf_s(charFecha, "%d", fechaHoy.wDay);
		strcat_s(fechaCompleta, charFecha);
		strcat_s(fechaCompleta, "/");
		sprintf_s(charFecha, "%d", fechaHoy.wMonth);
		strcat_s(fechaCompleta, charFecha);
		strcat_s(fechaCompleta, "/");
		sprintf_s(charFecha, "%d", fechaHoy.wYear);
		strcat_s(fechaCompleta, charFecha);


		SetDlgItemText(hwnd, 1058, fechaCompleta);
		SetDlgItemText(hwnd, 1057, NUsuario->usuarios_comercio);

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, NUsuario->usuarios_foto, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
		SendDlgItemMessage(hwnd, 1059, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		SetDlgItemText(hwnd, 1062, NUsuario->usuarios_nombre);
	}break;


	case WM_COMMAND: {
		switch (LOWORD(wParam))
		{
		case 1060: {//REGRESAR A LA VENTANA INICIAL
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG3), NULL, VentanaInicio);
		}break;
		case  1061: { //Boton de Guardar
			int CLetras = 0;
			int CNumeros = 0;
			int CCaracteres = 0;
			int CEspacios = 0;
			int lengthNombre = 0;
			int lengthEmail = 0;

			Cliente* Nuevo_cliente = new Cliente;
			GetDlgItemText(hwnd, 1053, Nuevo_cliente->Cliente_nombre, sizeof(Nuevo_cliente->Cliente_nombre));
			GetDlgItemText(hwnd, 1054, Nuevo_cliente->Cliente_email, sizeof(Nuevo_cliente->Cliente_email));
			GetDlgItemText(hwnd, 1057, Nuevo_cliente->Cliente_comercio, sizeof(Nuevo_cliente->Cliente_comercio));
			GetDlgItemText(hwnd, 1058, Nuevo_cliente->Cliente_fecha, sizeof(Nuevo_cliente->Cliente_fecha));
			Cliente_aux = Cliente_inicio;
			lengthNombre = strlen(Nuevo_cliente->Cliente_nombre);
			lengthEmail = strlen(Nuevo_cliente->Cliente_email);

			if (lengthNombre < 5 || lengthNombre > 50)
			{
				MessageBox(hwnd, "Favor de ingresar de 5 a 15 caracteres alfabeticos.", "NOMBRE", MB_OK | MB_ICONEXCLAMATION);
			}
			else {
				for (int i = 0; i < lengthNombre; i++)
				{
					if (ispunct(Nuevo_cliente->Cliente_nombre[i]))
					{
						CCaracteres++;
					}
					if (isdigit(Nuevo_cliente->Cliente_nombre[i]))
					{
						CNumeros++;
					}
				}
				if (CCaracteres != 0 || CNumeros != 0)
				{
					MessageBox(hwnd, "Favor de solo ingresar caracteres alfabeticos.", "NOMBRE", MB_OK | MB_ICONEXCLAMATION);
				}
				else
				{
					while (Cliente_aux != nullptr && strcmp(Cliente_aux->Cliente_nombre, Nuevo_cliente->Cliente_nombre) != 0) {//validacion mismmo nombre
						Cliente_aux = Cliente_aux->Cliente_sig;
					}
					if (Cliente_aux != nullptr) {
						MessageBox(hwnd, "Cliente ya existente, intente otro nombre", "AVISO", MB_OK | MB_ICONERROR);
					}
					else {
						if (lengthEmail < 5 || lengthEmail > 25)//validacion email
						{
							MessageBox(hwnd, "Favor de ingresar de 5 a 25 caracteres.", "EMAIL", MB_OK | MB_ICONEXCLAMATION);
						}
						else {
							for (int i = 0; i < lengthEmail; i++)
							{
								if (Nuevo_cliente->Cliente_email[i] == '@')
								{
									CCaracteres++;
								}
							}
							if (CCaracteres == 0)
							{
								MessageBox(hwnd, "Email invalido.", "EMAIL", MB_OK | MB_ICONEXCLAMATION);
							}
							else
							{
								Agregar_Cliente(Nuevo_cliente);
								delete Nuevo_cliente;
								Escribir_Binario_Cliente();

								SetDlgItemText(hwnd, 1053, "");
								SetDlgItemText(hwnd, 1054, "");
							}
						}
					}

				}
			}



		}break;
		}


	}
				  
	}
	return FALSE;
} 
LRESULT CALLBACK ListaClientes(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	char textoList[50] = { 0 };
	switch (msg) {
	case WM_INITDIALOG: {//LLENA LA LISTA DE USUARIOS CON LOS DATOS YA REGISTRADOS
		if (strcmp(NUsuario->usuarios_tipos, "Cajero") == 0)
		{
			EnableWindow(GetDlgItem(hwnd, 1072), false);
		}

		char Usuario_comercio_c[15];
		strcpy_s(Usuario_comercio_c, NUsuario->usuarios_comercio);

		Cliente_aux = Cliente_inicio;
		while (Cliente_aux != nullptr) {
			if (strcmp(Usuario_comercio_c, Cliente_aux->Cliente_comercio) == 0) {
				SendMessage(GetDlgItem(hwnd, 1073), LB_ADDSTRING, 0, (LPARAM)Cliente_aux->Cliente_nombre);
			}
			Cliente_aux = Cliente_aux->Cliente_sig;
		}
		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, NUsuario->usuarios_foto, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
		SendDlgItemMessage(hwnd, 1064, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		SetDlgItemText(hwnd, 1063, NUsuario->usuarios_nombre);
	}break;

	case WM_COMMAND: {
		if (LOWORD(wParam) == 1073) {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {

				int indice = 0;

				indice = SendDlgItemMessage(hwnd, 1073, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, 1073, LB_GETTEXT, indice, (LPARAM)textoList);

				Cliente_aux = Cliente_inicio;
				while (Cliente_aux != nullptr && strcmp(textoList, Cliente_aux->Cliente_nombre) != 0) {
					Cliente_aux = Cliente_aux->Cliente_sig;
				}
				if (Cliente_aux == nullptr) {

				}
				else
				{
					SetDlgItemText(hwnd, 1065, Cliente_aux->Cliente_nombre);
					SetDlgItemText(hwnd, 1066, Cliente_aux->Cliente_email);
					SetDlgItemText(hwnd, 1067, Cliente_aux->Cliente_comercio);
					SetDlgItemText(hwnd, 1068, Cliente_aux->Cliente_fecha);
				}

			}break;
			}
		}
		if (LOWORD(wParam) == 1029) {//Registrar
			int indice = 0;
			int CLetras = 0;
			int CNumeros = 0;
			int CCaracteres = 0;
			int CEspacios = 0;
			int lengthEmail = 0;

			char NEmail[51] = { 0 };
			//Usuario* newInfoUser = new Usuario;
			Cliente_aux = Cliente_inicio;
			indice = SendDlgItemMessage(hwnd, 1073, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, 1073, LB_GETTEXT, indice, (LPARAM)textoList);


			GetDlgItemText(hwnd, 1066, NEmail, sizeof(NEmail));

			//Modificar_Usuario(newInfoUser, textoList);


			while (Cliente_aux != nullptr && strcmp(Cliente_aux->Cliente_nombre, textoList) != 0) {

				Cliente_aux = Cliente_aux->Cliente_sig;

			}
			if (Cliente_aux == nullptr) {
				MessageBeep(MB_OK);
				MessageBox(hwnd, "No se encontro el cliente", "ADVERTENCIA", MB_OK | MB_ICONERROR);

			}
			else {
				lengthEmail = strlen(NEmail);
				if (lengthEmail < 5 || lengthEmail > 25)//validacion email
				{
					MessageBox(hwnd, "Favor de ingresar de 5 a 25 caracteres.", "EMAIL", MB_OK | MB_ICONEXCLAMATION);
				}
				else {
					for (int i = 0; i < lengthEmail; i++)
					{
						if (NEmail[i] == '@')
						{
							CCaracteres++;
						}
					}
					if (CCaracteres == 0)
					{
						MessageBox(hwnd, "Email invalido.", "EMAIL", MB_OK | MB_ICONEXCLAMATION);
					}
					else
					{
						int Opcion = MessageBox(0, "¿Seguro desea modificar el cliente?", "AVISO", MB_YESNO | MB_ICONQUESTION);
						switch (Opcion) {
						case IDYES: {
							strcpy_s(Cliente_aux->Cliente_email, NEmail);
							MessageBox(0, "Cliente modificado", "AVISO", MB_OK | MB_ICONINFORMATION);
							EndDialog(hwnd, 0);
							Escribir_Binario_Cliente();
							DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG9), hwnd, ListaClientes);
						}
								  break;
						}

					}

				}
			}



		}


		if (LOWORD(wParam) == 1072) {//Eliminar
			int indice = 0;

			indice = SendDlgItemMessage(hwnd, 1073, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd,1073, LB_GETTEXT, indice, (LPARAM)textoList);

			int Opcion = MessageBox(0, "¿Seguro que desea eliminar el cliente?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			if (Opcion == IDYES) {
				Eliminar_Cliente(textoList);

				SendMessage(GetDlgItem(hwnd, 1073), LB_DELETESTRING, (WPARAM)indice, 0);
				SendMessage(GetDlgItem(hPrinGlob, 1073), LB_RESETCONTENT, 0, 0);
				Escribir_Binario_Cliente();

				Cliente_aux = Cliente_inicio;
				while (Cliente_aux != nullptr) {
					SendMessage(GetDlgItem(hPrinGlob, 1073), LB_ADDSTRING, 0, (LPARAM)Cliente_aux->Cliente_comercio);
					Cliente_aux = Cliente_aux->Cliente_sig;
				}
			}

		}
		if (LOWORD(wParam) == 1074) {//modificar
			if (strcmp(NUsuario->usuarios_tipos, "Administrador") == 0)
			{
				EnableWindow(GetDlgItem(hwnd, 1066), true);
			}
		}

		if (LOWORD(wParam) == 1069) {
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG3), hwnd, VentanaInicio);
		}
		//switch (LOWORD(wParam)) {
		//case 1065: {//Regresar a menu
		//	EndDialog(hwnd, 0);
		//}break;
		//}
	}break;
	}
	return false;
}
LRESULT CALLBACK AltaConsumos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	char textoList[50] = { 0 };
	switch (msg) {

	case WM_INITDIALOG: {//rellena lista de CLIENTES
		char Usuario_comercio_c[15];
		strcpy_s(Usuario_comercio_c, NUsuario->usuarios_comercio);

		Cliente_aux = Cliente_inicio;
		while (Cliente_aux != nullptr) {
			if (strcmp(Usuario_comercio_c, Cliente_aux->Cliente_comercio) == 0) {
				SendMessage(GetDlgItem(hwnd, 1081), LB_ADDSTRING, 0, (LPARAM)Cliente_aux->Cliente_nombre);
			}
			Cliente_aux = Cliente_aux->Cliente_sig;
		}


		SYSTEMTIME fechaHoy;
		ZeroMemory(&fechaHoy, sizeof(fechaHoy));
		GetLocalTime(&fechaHoy);

		char fechaCompleta[50] = { 0 };
		char charFecha[50] = { 0 };
		float CantidadMonto = 0;

		char FC[50] = { 0 };
		char Fechaf[50] = { 0 };

		sprintf_s(charFecha, "%d", fechaHoy.wDay);
		strcat_s(fechaCompleta, charFecha);
		strcat_s(fechaCompleta, "/");
		sprintf_s(charFecha, "%d", fechaHoy.wMonth);
		strcat_s(fechaCompleta, charFecha);
		strcat_s(fechaCompleta, "/");
		sprintf_s(charFecha, "%d", fechaHoy.wYear);
		strcat_s(fechaCompleta, charFecha);

		sprintf_s(Fechaf, "%d", fechaHoy.wDay);
		strcat_s(FC, Fechaf);
		strcat_s(FC, "/");
		sprintf_s(Fechaf, "%d", fechaHoy.wMonth);
		strcat_s(FC, Fechaf);
		strcat_s(FC, "/");
		sprintf_s(Fechaf, "%d", fechaHoy.wYear);
		strcat_s(FC, Fechaf);
		strcat_s(FC, " ");
		sprintf_s(Fechaf, "%d", fechaHoy.wHour);
		strcat_s(FC, Fechaf);
		strcat_s(FC, ":");
		sprintf_s(Fechaf, "%d", fechaHoy.wMinute);
		strcat_s(FC, Fechaf);
		strcat_s(FC, ":");
		sprintf_s(Fechaf, "%d", fechaHoy.wSecond);
		strcat_s(FC, Fechaf);

		SetDlgItemText(hwnd, 1080, fechaCompleta);
		SetDlgItemText(hwnd, 1083, NUsuario->usuarios_comercio);
		SetDlgItemText(hwnd, 1082, FC);

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, NUsuario->usuarios_foto, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
		SendDlgItemMessage(hwnd, 1089, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		SetDlgItemText(hwnd, 1088, NUsuario->usuarios_nombre);
	}break;

	case WM_COMMAND: {
		if (LOWORD(wParam) == 1081) {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {

				int indice = 0;

				indice = SendDlgItemMessage(hwnd, 1081, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, 1081, LB_GETTEXT, indice, (LPARAM)textoList);

				Cliente_aux = Cliente_inicio;
				while (Cliente_aux != nullptr && strcmp(textoList, Cliente_aux->Cliente_nombre) != 0) {
					Cliente_aux = Cliente_aux->Cliente_sig;
				}
				if (Cliente_aux == nullptr) {

				}
				else
				{
					SetDlgItemText(hwnd, 1075, Cliente_aux->Cliente_nombre);
				}

			}break;
			}
		}

		if (LOWORD(wParam) == 1084) {
			int CLetras = 0;
			int CNumeros = 0;
			int CCaracteres = 0;
			int CEspacios = 0;
			int lengthMonto = 0;
			float CantidadMonto = 0;

			char Usuario_comercio_c[15];


			float CantidadDM = 0;
			float CantidadDescuento = 0;
			float CantidadSubtotal = 0;
			float CantidadTotal = 0;
			float Fmonto = 0;
			char Descuento[11] = { 0 };
			char Subtotal[11] = { 0 };
			char Total[11] = { 0 };


			int Validacion = 0;



			Consumo* Nuevo_consumo = new Consumo;
			GetDlgItemText(hwnd, 1083, Nuevo_consumo->Consumo_cliente, sizeof(Nuevo_consumo->Consumo_cliente));
			GetDlgItemText(hwnd, 1076, Nuevo_consumo->Consumo_monto, sizeof(Nuevo_consumo->Consumo_monto));
			GetDlgItemText(hwnd, 1083, Nuevo_consumo->Consumo_comercio, sizeof(Nuevo_consumo->Consumo_comercio));
			GetDlgItemText(hwnd, 1080, Nuevo_consumo->Consumo_fecha, sizeof(Nuevo_consumo->Consumo_fecha));
			GetDlgItemText(hwnd, 1082, Nuevo_consumo->Consumo_ID, sizeof(Nuevo_consumo->Consumo_ID));

			CantidadMonto = atof(Nuevo_consumo->Consumo_monto);
			lengthMonto = strlen(Nuevo_consumo->Consumo_monto);

			strcpy_s(Usuario_comercio_c, NUsuario->usuarios_comercio);
			char CCM[50] = { 0 };
			GetDlgItemText(hwnd, 1076, CCM, sizeof(CCM));
			Fmonto = atof(CCM);
			lengthMonto = strlen(CCM);

			if (strcmp(Nuevo_consumo->Consumo_cliente, "") == 0)
			{
				MessageBox(hwnd, "Favor de seleccionar un cliente de la lista dando doble click.", "CLIENTE", MB_OK | MB_ICONEXCLAMATION);
			}
			else {
				if (CantidadMonto <= 0)
				{
					MessageBox(hwnd, "Favor de ingresar numeros mayores a 0.", "MONTO", MB_OK | MB_ICONEXCLAMATION);
					SetDlgItemText(hwnd, 1079, "");
					SetDlgItemText(hwnd, 1078, "");
					SetDlgItemText(hwnd, 1087, "");
				}
				else {
					for (int i = 0; i < lengthMonto; i++)
					{
						if (ispunct(Nuevo_consumo->Consumo_monto[i]))
						{
							CCaracteres++;
						}
						if (isalpha(Nuevo_consumo->Consumo_monto[i]))
						{
							CLetras++;
						}
						if (isspace(Nuevo_consumo->Consumo_monto[i]))
						{
							CEspacios++;
						}
						if (Nuevo_consumo->Consumo_monto[i] == '.')
						{
							CCaracteres = CCaracteres - 1;
						}
					}
					if (CCaracteres != 0 || CLetras != 0 || CEspacios != 0)
					{
						MessageBox(hwnd, "Favor de solo ingresar numeros.", "MONTO", MB_OK | MB_ICONEXCLAMATION);
						SetDlgItemText(hwnd, 1079, "");
						SetDlgItemText(hwnd, 1078, "");
						SetDlgItemText(hwnd, 1087, "");
					}
					else
					{
						Promocion_aux = Promocion_inicio;
						while (Promocion_aux != nullptr) {
							if (strcmp(Usuario_comercio_c, Promocion_aux->Promocion_comercio) == 0) //si el comercio del usuario coincide con el comercio promocon
							{
								if (strcmp(CCM, Promocion_aux->Promocion_monto) == 0) {//si el monto consumo coincide con el promocion

									if (strcmp(Promocion_aux->Promocion_estatus, "Activa") == 0) {

										CantidadMonto = atof(CCM);
										CantidadDescuento = atof(Promocion_aux->Promocion_descuento);
										CantidadDM = CantidadMonto * (CantidadDescuento / 100);
										CantidadDescuento = (100 - CantidadDescuento) / 100;
										CantidadSubtotal = CantidadMonto * CantidadDescuento;
										CantidadTotal = CantidadSubtotal + (CantidadSubtotal * 0.16);

										sprintf_s(Descuento, "%.2f", CantidadDM);
										SetDlgItemText(hwnd, 1078, Descuento);

										sprintf_s(Subtotal, "%.2f", CantidadSubtotal);
										SetDlgItemText(hwnd, 1087, Subtotal);

										sprintf_s(Total, "%.2f", CantidadTotal);
										SetDlgItemText(hwnd, 1079, Total);
										Validacion = 1;
									}
								}
							}
							Promocion_aux = Promocion_aux->Promocion_sig;
						}
						if (Validacion == 0)
						{
							CantidadMonto = atof(CCM);
							CantidadDM = 0;
							CantidadSubtotal = CantidadMonto;
							CantidadTotal = CantidadSubtotal + (CantidadSubtotal * 0.16);
							sprintf_s(Descuento, "%.2f", CantidadDM);
							SetDlgItemText(hwnd, 1078, Descuento);

							sprintf_s(Subtotal, "%.2f", CantidadSubtotal);
							SetDlgItemText(hwnd, 1087, Subtotal);

							sprintf_s(Total, "%.2f", CantidadTotal);
							SetDlgItemText(hwnd, 1079, Total);
						}

						GetDlgItemText(hwnd, 1087, Nuevo_consumo->Consumo_subtotal, sizeof(Nuevo_consumo->Consumo_subtotal));
						GetDlgItemText(hwnd, 1078, Nuevo_consumo->Consumo_descuento, sizeof(Nuevo_consumo->Consumo_descuento));
						GetDlgItemText(hwnd, 1079, Nuevo_consumo->Consumo_total, sizeof(Nuevo_consumo->Consumo_total));

						Agregar_Consumo(Nuevo_consumo);
						delete Nuevo_consumo;
						Escribir_Binario_Consumo();

						SYSTEMTIME fechaHoy;
						ZeroMemory(&fechaHoy, sizeof(fechaHoy));
						GetLocalTime(&fechaHoy);
						SetDlgItemText(hwnd, 1079, "");
						SetDlgItemText(hwnd, 1078, "");
						SetDlgItemText(hwnd, 1087, "");
						SetDlgItemText(hwnd, 1075, "");
						SetDlgItemText(hwnd, 1076, "");
						char FC[50] = { 0 };
						char Fechaf[50] = { 0 };
						sprintf_s(Fechaf, "%d", fechaHoy.wDay);
						strcat_s(FC, Fechaf);
						strcat_s(FC, "/");
						sprintf_s(Fechaf, "%d", fechaHoy.wMonth);
						strcat_s(FC, Fechaf);
						strcat_s(FC, "/");
						sprintf_s(Fechaf, "%d", fechaHoy.wYear);
						strcat_s(FC, Fechaf);
						strcat_s(FC, " ");
						sprintf_s(Fechaf, "%d", fechaHoy.wHour);
						strcat_s(FC, Fechaf);
						strcat_s(FC, ":");
						sprintf_s(Fechaf, "%d", fechaHoy.wMinute);
						strcat_s(FC, Fechaf);
						strcat_s(FC, ":");
						sprintf_s(Fechaf, "%d", fechaHoy.wSecond);
						strcat_s(FC, Fechaf);
						SetDlgItemText(hwnd, 1082, FC);
					}
				}
			}

		}
		if (LOWORD(wParam) == 1086) { //calcular
			char Usuario_comercio_c[15];
			float CantidadMonto = 0;
			float CantidadDM = 0;
			float CantidadDescuento = 0;
			float CantidadSubtotal = 0;
			float CantidadTotal = 0;
			float Fmonto = 0;
			char Descuento[11] = { 0 };
			char Subtotal[11] = { 0 };
			char Total[11] = { 0 };

			int CLetras = 0;
			int CNumeros = 0;
			int CCaracteres = 0;
			int CEspacios = 0;
			int lengthMonto = 0;
			int Validacion = 0;


			strcpy_s(Usuario_comercio_c, NUsuario->usuarios_comercio);
			char CCM[50] = { 0 };
			GetDlgItemText(hwnd, 1076, CCM, sizeof(CCM));
			Fmonto = atof(CCM);
			lengthMonto = strlen(CCM);



			if (Fmonto <= 0)
			{
				MessageBox(hwnd, "Favor de ingresar numeros mayores a 0.", "MONTO", MB_OK | MB_ICONEXCLAMATION);
				SetDlgItemText(hwnd, 1079, "");
				SetDlgItemText(hwnd, 1078, "");
				SetDlgItemText(hwnd, 1087, "");
			}
			else {
				for (int i = 0; i < lengthMonto; i++)
				{
					if (ispunct(CCM[i]))
					{
						CCaracteres++;
					}
					if (isalpha(CCM[i]))
					{
						CLetras++;
					}
					if (isspace(CCM[i]))
					{
						CEspacios++;
					}
					if (CCM[i] == '.')
					{
						CCaracteres = CCaracteres - 1;
					}
				}
				if (CCaracteres != 0 || CLetras != 0 || CEspacios != 0)
				{
					MessageBox(hwnd, "Favor de solo ingresar numeros.", "MONTO", MB_OK | MB_ICONEXCLAMATION);
				}
				else
				{
					Promocion_aux = Promocion_inicio;
					while (Promocion_aux != nullptr) {
						if (strcmp(Usuario_comercio_c, Promocion_aux->Promocion_comercio) == 0) //si el comercio del usuario coincide con el comercio promocon
						{
							if (strcmp(CCM, Promocion_aux->Promocion_monto) == 0) {//si el monto consumo coincide con el promocion

								if (strcmp(Promocion_aux->Promocion_estatus, "Activa") == 0) {

									CantidadMonto = atof(CCM);
									CantidadDescuento = atof(Promocion_aux->Promocion_descuento);
									CantidadDM = CantidadMonto * (CantidadDescuento / 100);
									CantidadDescuento = (100 - CantidadDescuento) / 100;
									CantidadSubtotal = CantidadMonto * CantidadDescuento;
									CantidadTotal = CantidadSubtotal + (CantidadSubtotal * 0.16);

									sprintf_s(Descuento, "%.2f", CantidadDM);
									SetDlgItemText(hwnd, 107, Descuento);

									sprintf_s(Subtotal, "%.2f", CantidadSubtotal);
									SetDlgItemText(hwnd, 93, Subtotal);

									sprintf_s(Total, "%.2f", CantidadTotal);
									SetDlgItemText(hwnd, 94, Total);
									Validacion = 1;
								}
							}
						}
						Promocion_aux = Promocion_aux->Promocion_sig;
					}
					if (Validacion == 0)
					{
						CantidadMonto = atof(CCM);
						CantidadDM = 0;
						CantidadSubtotal = CantidadMonto;
						CantidadTotal = CantidadSubtotal + (CantidadSubtotal * 0.16);
						sprintf_s(Descuento, "%.2f", CantidadDM);
						SetDlgItemText(hwnd, 1078, Descuento);

						sprintf_s(Subtotal, "%.2f", CantidadSubtotal);
						SetDlgItemText(hwnd, 1087, Subtotal);

						sprintf_s(Total, "%.2f", CantidadTotal);
						SetDlgItemText(hwnd, 1079, Total);
					}
				}


			}

			/*}*/


		}

		if (LOWORD(wParam) == 1085) {
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG3), hwnd, VentanaInicio);
		}

	}break;
	}
	return false;
}
LRESULT CALLBACK ListaConsumos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	char textoList[50] = { 0 };
	switch (msg) {
		
	case WM_INITDIALOG: {
		char Usuario_comercio_c[15];
		strcpy_s(Usuario_comercio_c, NUsuario->usuarios_comercio);

		Consumo_aux = Consumo_inicio;
		while (Consumo_aux != nullptr) {
			if (strcmp(Usuario_comercio_c, Consumo_aux->Consumo_comercio) == 0) {
				SendMessage(GetDlgItem(hwnd, 1097), LB_ADDSTRING, 0, (LPARAM)Consumo_aux->Consumo_ID);
			}
			Consumo_aux = Consumo_aux->Consumo_sig;
		}

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, NUsuario->usuarios_foto, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
		SendDlgItemMessage(hwnd, 1098, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		SetDlgItemText(hwnd, 1099, NUsuario->usuarios_nombre);
	}break;
	case WM_COMMAND: {
		if (LOWORD(wParam) == 1097) {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {

				int indice = 0;

				indice = SendDlgItemMessage(hwnd, 1097, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, 1097, LB_GETTEXT, indice, (LPARAM)textoList);

				Consumo_aux = Consumo_inicio;
				while (Consumo_aux != nullptr && strcmp(textoList, Consumo_aux->Consumo_ID) != 0) {
					Consumo_aux = Consumo_aux->Consumo_sig;
				}
				if (Consumo_aux == nullptr) {

				}
				else
				{
					SetDlgItemText(hwnd, 1090, Consumo_aux->Consumo_cliente);
					SetDlgItemText(hwnd, 1091, Consumo_aux->Consumo_monto);
					SetDlgItemText(hwnd, 1093, Consumo_aux->Consumo_descuento);
					SetDlgItemText(hwnd, 1094, Consumo_aux->Consumo_total);
					SetDlgItemText(hwnd, 1092, Consumo_aux->Consumo_subtotal);
					SetDlgItemText(hwnd, 1095, Consumo_aux->Consumo_fecha);
				}

			}break;
			}
		}
		if (LOWORD(wParam) == 1096) {
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG3), hwnd, VentanaInicio);
		} 
		
	}break;
	}
	return FALSE;
}
LRESULT CALLBACK FiltroConsumos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	char textoList[50] = { 0 };
	switch (msg) {
	case WM_INITDIALOG: {//rellena lista de usuarios
		char Usuario_comercio_c[15];
		strcpy_s(Usuario_comercio_c, NUsuario->usuarios_comercio);

		Cliente_aux = Cliente_inicio;
		while (Cliente_aux != nullptr) {
			if (strcmp(Usuario_comercio_c, Cliente_aux->Cliente_comercio) == 0) {
				SendMessage(GetDlgItem(hwnd, 1121), LB_ADDSTRING, 0, (LPARAM)Cliente_aux->Cliente_nombre);
			}
			Cliente_aux = Cliente_aux->Cliente_sig;
		}

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, NUsuario->usuarios_foto, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
		SendDlgItemMessage(hwnd, 1118, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		SetDlgItemText(hwnd, 1119, NUsuario->usuarios_nombre);
	}break;

	case WM_COMMAND: {
		if (LOWORD(wParam) == 1121) {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {

				SendMessage(GetDlgItem(hwnd, 1122), LB_RESETCONTENT, 0, 0);
				SetDlgItemText(hwnd, 1112, "");
				SetDlgItemText(hwnd, 1113, "");
				SetDlgItemText(hwnd, 1114, "");
				SetDlgItemText(hwnd, 1115, "");
				SetDlgItemText(hwnd, 1117, "");
				SetDlgItemText(hwnd, 1116, "");
				int indice = 0;

				indice = SendDlgItemMessage(hwnd, 1121, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, 1121, LB_GETTEXT, indice, (LPARAM)textoList);

				Consumo_aux = Consumo_inicio;
				while (Consumo_aux != nullptr) {
					if (strcmp(textoList, Consumo_aux->Consumo_cliente) == 0)
					{
						SendMessage(GetDlgItem(hwnd, 1122), LB_ADDSTRING, 0, (LPARAM)Consumo_aux->Consumo_ID);
					}
					Consumo_aux = Consumo_aux->Consumo_sig;
				}

			}break;
			}
		}

		if (LOWORD(wParam) == 1122) {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {



				int indice = 0;

				indice = SendDlgItemMessage(hwnd, 1122, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, 1122, LB_GETTEXT, indice, (LPARAM)textoList);

				Consumo_aux = Consumo_inicio;
				while (Consumo_aux != nullptr && strcmp(textoList, Consumo_aux->Consumo_ID) != 0) {
					Consumo_aux = Consumo_aux->Consumo_sig;
				}
				if (Consumo_aux == nullptr) {

				}
				else
				{
					SetDlgItemText(hwnd, 1112, Consumo_aux->Consumo_cliente);
					SetDlgItemText(hwnd, 1113, Consumo_aux->Consumo_monto);
					SetDlgItemText(hwnd, 1114, Consumo_aux->Consumo_subtotal);
					SetDlgItemText(hwnd, 1115, Consumo_aux->Consumo_descuento);
					SetDlgItemText(hwnd, 1117, Consumo_aux->Consumo_fecha);
					SetDlgItemText(hwnd, 1116, Consumo_aux->Consumo_total);
				}

			}break;
			}
		}

		if (LOWORD(wParam) == 1120) {
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG3), hwnd, VentanaInicio);
		}

	}break;
	}
	return false;
}
LRESULT CALLBACK AltaPromocion(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_INITDIALOG: {
		SYSTEMTIME fechaHoy;
		ZeroMemory(&fechaHoy, sizeof(fechaHoy));
		GetLocalTime(&fechaHoy);

		char fechaCompleta[11] = { 0 };
		char charFecha[5] = { 0 };

		sprintf_s(charFecha, "%d", fechaHoy.wDay);
		strcat_s(fechaCompleta, charFecha);
		strcat_s(fechaCompleta, "/");
		sprintf_s(charFecha, "%d", fechaHoy.wMonth);
		strcat_s(fechaCompleta, charFecha);
		strcat_s(fechaCompleta, "/");
		sprintf_s(charFecha, "%d", fechaHoy.wYear);
		strcat_s(fechaCompleta, charFecha);


		SetDlgItemText(hwnd, 1128, fechaCompleta);
		SetDlgItemText(hwnd, 1124, NUsuario->usuarios_comercio);

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, NUsuario->usuarios_foto, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
		SendDlgItemMessage(hwnd, 1131, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		SetDlgItemText(hwnd, 1132, NUsuario->usuarios_nombre);
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case 1137: {//Regresar a menu
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG3), hwnd, VentanaInicio);
		}break;
		case  1134: { //Boton de Guardar
			int CLetras = 0;
			int CNumeros = 0;
			int CCaracteres = 0;
			int CEspacios = 0;
			int lengthNombre = 0;
			int lengthMonto = 0;
			int lengthDescuento = 0;
			float CantidadMonto = 0;
			float Descuento = 0;
			Promocion* Nuevo_promocion = new Promocion;
			GetDlgItemText(hwnd, 1123, Nuevo_promocion->Promocion_nombre, sizeof(Nuevo_promocion->Promocion_nombre));
			GetDlgItemText(hwnd, 1126, Nuevo_promocion->Promocion_descuento, sizeof(Nuevo_promocion->Promocion_descuento));
			GetDlgItemText(hwnd, 1125, Nuevo_promocion->Promocion_monto, sizeof(Nuevo_promocion->Promocion_monto));
			GetDlgItemText(hwnd, 1128, Nuevo_promocion->Promocion_fecha, sizeof(Nuevo_promocion->Promocion_fecha));
			GetDlgItemText(hwnd, 1124, Nuevo_promocion->Promocion_comercio, sizeof(Nuevo_promocion->Promocion_comercio));
			CantidadMonto = atof(Nuevo_promocion->Promocion_monto);
			Descuento = atof(Nuevo_promocion->Promocion_descuento);
			lengthNombre = strlen(Nuevo_promocion->Promocion_nombre);
			lengthMonto = strlen(Nuevo_promocion->Promocion_monto);
			lengthDescuento = strlen(Nuevo_promocion->Promocion_descuento);


			if (lengthNombre < 5 || lengthNombre > 50)
			{
				MessageBox(hwnd, "Favor de ingresar de 5 a 15 caracteres alfabeticos y/o espacios.", "NOMBRE", MB_OK | MB_ICONEXCLAMATION);
			}
			else {
				for (int i = 0; i < lengthNombre; i++)
				{
					if (ispunct(Nuevo_promocion->Promocion_nombre[i]))
					{
						CCaracteres++;
					}
					if (isdigit(Nuevo_promocion->Promocion_nombre[i]))
					{
						CNumeros++;
					}
				}
				if (CCaracteres != 0 || CNumeros != 0)
				{
					MessageBox(hwnd, "Favor de solo ingresar caracteres alfabeticos y/o espacios.", "NOMBRE", MB_OK | MB_ICONEXCLAMATION);
				}
				else
				{
					if (CantidadMonto < 100)
					{
						MessageBox(hwnd, "Solo numeros no negativos mayores a 100", "MONTO", MB_OK | MB_ICONEXCLAMATION);
					}
					else {
						for (int i = 0; i < lengthMonto; i++)
						{
							if (ispunct(Nuevo_promocion->Promocion_monto[i]))
							{
								CCaracteres++;
							}
							if (isalpha(Nuevo_promocion->Promocion_monto[i]))
							{
								CLetras++;
							}
							if (isspace(Nuevo_promocion->Promocion_monto[i]))
							{
								CEspacios++;
							}
							if (Nuevo_promocion->Promocion_monto[i] == '.')
							{
								CCaracteres = CCaracteres - 1;
							}
						}
						if (CCaracteres != 0 || CLetras != 0 || CEspacios != 0)
						{
							MessageBox(hwnd, "Favor de solo ingresar numeros.", "MONTO", MB_OK | MB_ICONEXCLAMATION);
						}
						else {
							if (Descuento <= 0 || Descuento > 100)
							{
								MessageBox(hwnd, "Ingresar cantidad decimal, mayor a 0 y menor a 100", "DESCUENTO", MB_OK | MB_ICONEXCLAMATION);
							}
							else {
								for (int i = 0; i < lengthDescuento; i++)
								{
									if (ispunct(Nuevo_promocion->Promocion_descuento[i]))
									{
										CCaracteres++;
									}
									if (isalpha(Nuevo_promocion->Promocion_descuento[i]))
									{
										CLetras++;
									}
									if (isspace(Nuevo_promocion->Promocion_descuento[i]))
									{
										CEspacios++;
									}
									if (Nuevo_promocion->Promocion_descuento[i] == '.')
									{
										CCaracteres = CCaracteres - 1;
									}
								}
								if (CCaracteres != 0 || CLetras != 0 || CEspacios != 0)
								{
									MessageBox(hwnd, "Favor de solo ingresar numeros.", "DESCUENTO", MB_OK | MB_ICONEXCLAMATION);
								}
								else {
									if (IsDlgButtonChecked(hwnd, 1129) == BST_UNCHECKED && IsDlgButtonChecked(hwnd, 1130) == BST_UNCHECKED)
									{
										MessageBox(hwnd, "Favor de seleccionar un estatus.", "ESTATUS", MB_OK | MB_ICONEXCLAMATION);
									}
									else {
										if (IsDlgButtonChecked(hwnd, 1129) == BST_CHECKED)
										{
											strcpy_s(Nuevo_promocion->Promocion_estatus, "Activa");
										}
										if (IsDlgButtonChecked(hwnd, 1130) == BST_CHECKED)
										{
											strcpy_s(Nuevo_promocion->Promocion_estatus, "Inactiva");
										}
										Agregar_Promocion(Nuevo_promocion);
										delete Nuevo_promocion;
										Escribir_Binario_Promocion();

										SetDlgItemText(hwnd, 1123, "");
										SetDlgItemText(hwnd, 1126, "");
										SetDlgItemText(hwnd, 1125, "");
										SendDlgItemMessage(hwnd, 1130, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
										SendDlgItemMessage(hwnd, 1129, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);

									}
								}

							}
						}
					}
				}
			}



		}break;

		}
		/*return FALSE;*/
	}
				  
	}
	return FALSE;
}



LRESULT CALLBACK ListaPromocion(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_INITDIALOG:
	{
	}
	break;
	}
	return FALSE;
}
	
	


void Agregar_usuario(Usuario* nuevo_u) {
	if (Usuario_inicio == nullptr) {
		Usuario_inicio = new Usuario;
		Usuario_aux = Usuario_inicio;

		Usuario_aux->Usuario_sig = nullptr;
		Usuario_aux->Usuario_ant = nullptr;

		strcpy_s(Usuario_aux->usuarios_nombre, nuevo_u->usuarios_nombre);
		strcpy_s(Usuario_aux->usuarios_nombrecompleto, nuevo_u->usuarios_nombrecompleto);
		strcpy_s(Usuario_aux->usuarios_foto, nuevo_u->usuarios_foto);
		strcpy_s(Usuario_aux->usuarios_contrasenia, nuevo_u->usuarios_contrasenia);
		strcpy_s(Usuario_aux->usuarios_comercio, nuevo_u->usuarios_comercio);
		strcpy_s(Usuario_aux->usuarios_foto, nuevo_u->usuarios_foto);
		strcpy_s(Usuario_aux->usuarios_direccion, nuevo_u->usuarios_direccion);
		strcpy_s(Usuario_aux->usuarios_primeregistro, nuevo_u->usuarios_primeregistro);
		strcpy_s(Usuario_aux->usuarios_tipos, nuevo_u->usuarios_tipos);
		/* Usuario_aux->usuarios_tipos = nuevo_u->usuarios_tipos;*/
	}
	else {
		Usuario_aux = Usuario_inicio;

		while (Usuario_aux->Usuario_sig != nullptr)
		{
			Usuario_aux = Usuario_aux->Usuario_sig; 
		}

		Usuario_aux->Usuario_sig = new Usuario;

		Usuario_aux->Usuario_sig->Usuario_sig = nullptr;
		Usuario_aux->Usuario_sig->Usuario_ant = Usuario_aux;

		Usuario_aux = Usuario_aux->Usuario_sig;

		strcpy_s(Usuario_aux->usuarios_nombre, nuevo_u->usuarios_nombre);
		strcpy_s(Usuario_aux->usuarios_contrasenia, nuevo_u->usuarios_contrasenia);
		strcpy_s(Usuario_aux->usuarios_nombrecompleto, nuevo_u->usuarios_nombrecompleto);
		strcpy_s(Usuario_aux->usuarios_comercio, nuevo_u->usuarios_comercio);
		strcpy_s(Usuario_aux->usuarios_foto, nuevo_u->usuarios_foto);
		/*Usuario_aux->usuarios_tipos = nuevo_u->usuarios_tipos;*/
		strcpy_s(Usuario_aux->usuarios_direccion, nuevo_u->usuarios_direccion);
		strcpy_s(Usuario_aux->usuarios_tipos, nuevo_u->usuarios_tipos);
		strcpy_s(Usuario_aux->usuarios_primeregistro, nuevo_u->usuarios_primeregistro);

	}
	MessageBox(0, "Usuario registrado", "AVISO", MB_OK);
}

void Modificar_Usuario(Usuario* newInfo, char nomUsuario[50]) {

	Usuario_aux = Usuario_inicio;

	if (Usuario_aux == nullptr)
	{
		MessageBox(0, "No hay Usuario registrados", "AVISO", MB_OK | MB_ICONINFORMATION);
	}
	else {
		while (Usuario_aux != nullptr && strcmp(Usuario_aux->usuarios_nombre, nomUsuario) != 0) {

			Usuario_aux = Usuario_aux->Usuario_sig;
		}
		if (Usuario_aux == nullptr) {
			MessageBox(0, "Usuario no encontrado", "AVISO", MB_OK | MB_ICONINFORMATION);
		}
		else {
			strcpy_s(Usuario_aux->usuarios_nombre, newInfo->usuarios_nombre);


			MessageBox(0, "Usuario modificado", "AVISO", MB_OK | MB_ICONINFORMATION);
		}
	}

}

void Eliminar_Usuario(char nomUsuario[50]) {
	Usuario_aux = Usuario_inicio;

	if (Usuario_aux == nullptr)
	{
		MessageBox(0, "No hay Usuario registrados", "AVISO", MB_OK | MB_ICONINFORMATION);
	}
	else {
		while (Usuario_aux != nullptr && strcmp(Usuario_aux->usuarios_nombre, nomUsuario) != 0) {

			Usuario_aux = Usuario_aux->Usuario_sig;
		}
		if (Usuario_aux == nullptr) {
			MessageBox(0, "Usuario no encontrado", "AVISO", MB_OK);
		}
		else if (Usuario_aux == Usuario_inicio) {	//El nodo es el primero 

			if (Usuario_aux->Usuario_sig == nullptr) {

				Usuario_inicio = nullptr;
				delete Usuario_aux;
				Usuario_aux = Usuario_inicio;
			}
			else {
				Usuario_inicio = Usuario_inicio->Usuario_sig;
				Usuario_aux->Usuario_sig->Usuario_ant = nullptr;
				delete Usuario_aux;
				Usuario_aux = Usuario_inicio;

			}

			MessageBox(0, "Usuario eliminado", "AVISO", MB_OK | MB_ICONINFORMATION);
		}
		else {

			if (Usuario_aux->Usuario_sig == nullptr) { //Si el nodo es el último
				Usuario_aux->Usuario_ant->Usuario_sig = nullptr;
				delete Usuario_aux;
				Usuario_aux = Usuario_inicio;
			}
			else {
				Usuario_aux->Usuario_sig->Usuario_ant = Usuario_aux->Usuario_ant;
				Usuario_aux->Usuario_ant->Usuario_sig = Usuario_aux->Usuario_sig;
				delete Usuario_aux;
				Usuario_aux = Usuario_inicio;
			}

			MessageBox(0, "Usuario eliminado", "AVISO", MB_OK | MB_ICONINFORMATION);

		}
	}

}

void Escribir_Binario_Usuario()
{
	Usuario_aux = Usuario_inicio;

	ofstream escribirUsuario;
	escribirUsuario.open("C:\\Users\\PC\\Documents\\Datos binario\\Usuario.bin", ios::out | ios::binary | ios::trunc);

	if (escribirUsuario.is_open()) {
		while (Usuario_aux != nullptr)
		{
			escribirUsuario.write((char*)Usuario_aux, sizeof(Usuario));
			Usuario_aux = Usuario_aux->Usuario_sig;
		}

		escribirUsuario.close();
	}
	else
	{
		system("PAUSE");
	}
}

void Leer_Binario_Usuario()
{
	ifstream archivoUsuario;
	archivoUsuario.open("C:\\Users\\PC\\Documents\\Datos binario\\Usuario.bin", ios::in | ios::binary);


	if (archivoUsuario.is_open()) {

		Usuario* Usuario_leido = new Usuario;

		while (!archivoUsuario.read((char*)Usuario_leido, sizeof(Usuario)).eof()) {

			if (Usuario_inicio == nullptr) {
				Usuario_inicio = Usuario_leido;
				Usuario_inicio->Usuario_sig = nullptr;
				Usuario_inicio->Usuario_ant = nullptr;
				Usuario_aux = Usuario_inicio;
			}
			else {
				Usuario_aux->Usuario_sig = Usuario_leido;
				Usuario_aux->Usuario_sig->Usuario_ant = Usuario_aux;
				Usuario_aux = Usuario_aux->Usuario_sig;
				Usuario_aux->Usuario_sig = nullptr;
				//fin = aux;
			}

			Usuario_leido = new Usuario;
		}
		archivoUsuario.close();
		delete Usuario_leido;
	}
	else {
		system("PAUSE");
	}
}

void Agregar_Cliente(Cliente* nuevo) {


	if (Cliente_inicio == nullptr) {
		Cliente_inicio = new Cliente;
		Cliente_aux = Cliente_inicio;

		Cliente_aux->Cliente_sig = nullptr;
		Cliente_aux->Cliente_ant = nullptr;

		strcpy_s(Cliente_aux->Cliente_nombre, nuevo->Cliente_nombre);
		strcpy_s(Cliente_aux->Cliente_email, nuevo->Cliente_email);
		strcpy_s(Cliente_aux->Cliente_comercio, nuevo->Cliente_comercio);
		strcpy_s(Cliente_aux->Cliente_fecha, nuevo->Cliente_fecha);

	}
	else {
		Cliente_aux = Cliente_inicio;
		while (Cliente_aux->Cliente_sig != nullptr)
		{
			Cliente_aux = Cliente_aux->Cliente_sig;
		}

		Cliente_aux->Cliente_sig = new Cliente;

		Cliente_aux->Cliente_sig->Cliente_sig = nullptr;
		Cliente_aux->Cliente_sig->Cliente_ant = Cliente_aux;

		Cliente_aux = Cliente_aux->Cliente_sig;

		strcpy_s(Cliente_aux->Cliente_nombre, nuevo->Cliente_nombre);
		strcpy_s(Cliente_aux->Cliente_email, nuevo->Cliente_email);
		strcpy_s(Cliente_aux->Cliente_comercio, nuevo->Cliente_comercio);
		strcpy_s(Cliente_aux->Cliente_fecha, nuevo->Cliente_fecha);
		Cliente_aux->Cliente_sig = nullptr;
	}
	MessageBox(0, "Cliente registrado", "AVISO", MB_OK);
}

void Escribir_Binario_Cliente()
{
	Cliente_aux = Cliente_inicio;

	ofstream escribirCliente;
	escribirCliente.open("C:\\Users\\PC\\Documents\\Datos binario\\Cliente.bin", ios::out | ios::binary | ios::trunc);

	if (escribirCliente.is_open()) {
		while (Cliente_aux != nullptr)
		{
			escribirCliente.write((char*)Cliente_aux, sizeof(Cliente));
			Cliente_aux = Cliente_aux->Cliente_sig;
		}

		escribirCliente.close();
	}
	else
	{
		system("PAUSE");
	}
}

void Leer_Binario_Cliente()
{
	ifstream archivoCliente;
	archivoCliente.open("C:\\Users\\PC\\Documents\\Datos binario\\Cliente.bin", ios::in | ios::binary);


	if (archivoCliente.is_open()) {

		Cliente* Cliente_leido = new Cliente;

		while (!archivoCliente.read((char*)Cliente_leido, sizeof(Cliente)).eof()) {

			if (Cliente_inicio == nullptr) {
				Cliente_inicio = Cliente_leido;
				Cliente_inicio->Cliente_sig = nullptr;
				Cliente_inicio->Cliente_ant = nullptr;
				Cliente_aux = Cliente_inicio;
			}
			else {
				Cliente_aux->Cliente_sig = Cliente_leido;
				Cliente_aux->Cliente_sig->Cliente_ant = Cliente_aux;
				Cliente_aux = Cliente_aux->Cliente_sig;
				Cliente_aux->Cliente_sig = nullptr;
				//fin = aux;
			}

			Cliente_leido = new Cliente;
		}
		archivoCliente.close();
		delete Cliente_leido;
	}
	else {
		system("PAUSE");
	}
}

void Eliminar_Cliente(char nomCliente[50]) {
	Cliente_aux = Cliente_inicio;

	if (Cliente_aux == nullptr)
	{
		MessageBox(0, "No hay clientes registrados", "AVISO", MB_OK | MB_ICONINFORMATION);
	}
	else {
		while (Cliente_aux != nullptr && strcmp(Cliente_aux->Cliente_nombre, nomCliente) != 0) {

			Cliente_aux = Cliente_aux->Cliente_sig;
		}
		if (Cliente_aux == nullptr) {
			MessageBox(0, "Cliente no encontrado", "AVISO", MB_OK);
		}
		else if (Cliente_aux == Cliente_inicio) {	//El nodo es el primero 

			if (Cliente_aux->Cliente_sig == nullptr) {

				Cliente_inicio = nullptr;
				delete Cliente_aux;
				Cliente_aux = Cliente_inicio;
			}
			else {
				Cliente_inicio = Cliente_inicio->Cliente_sig;
				Cliente_aux->Cliente_sig->Cliente_ant = nullptr;
				delete Cliente_aux;
				Cliente_aux = Cliente_inicio;

			}

			MessageBox(0, "Cliente eliminado", "AVISO", MB_OK | MB_ICONINFORMATION);
		}
		else {

			if (Cliente_aux->Cliente_sig == nullptr) { //Si el nodo es el último
				Cliente_aux->Cliente_ant->Cliente_sig = nullptr;
				delete Cliente_aux;
				Cliente_aux = Cliente_inicio;
			}
			else {
				Cliente_aux->Cliente_sig->Cliente_ant = Cliente_aux->Cliente_ant;
				Cliente_aux->Cliente_ant->Cliente_sig = Cliente_aux->Cliente_sig;
				delete Cliente_aux;
				Cliente_aux = Cliente_inicio;
			}

			MessageBox(0, "Cliente eliminado", "AVISO", MB_OK | MB_ICONINFORMATION);

		}
	}

}
void Agregar_Consumo(Consumo* nuevo) {


	if (Consumo_inicio == nullptr) {
		Consumo_inicio = new Consumo;
		Consumo_aux = Consumo_inicio;

		Consumo_aux->Consumo_sig = nullptr;
		Consumo_aux->Consumo_ant = nullptr;

		strcpy_s(Consumo_aux->Consumo_cliente, nuevo->Consumo_cliente);
		strcpy_s(Consumo_aux->Consumo_fecha, nuevo->Consumo_fecha);
		strcpy_s(Consumo_aux->Consumo_monto, nuevo->Consumo_monto);
		strcpy_s(Consumo_aux->Consumo_descuento, nuevo->Consumo_descuento);
		strcpy_s(Consumo_aux->Consumo_subtotal, nuevo->Consumo_subtotal);
		strcpy_s(Consumo_aux->Consumo_total, nuevo->Consumo_total);
		strcpy_s(Consumo_aux->Consumo_comercio, nuevo->Consumo_comercio);
		strcpy_s(Consumo_aux->Consumo_ID, nuevo->Consumo_ID);
		
	}
	else {
		Consumo_aux = Consumo_inicio;

		while (Consumo_aux->Consumo_sig != nullptr)
		{
			Consumo_aux = Consumo_aux->Consumo_sig;
		}

		Consumo_aux->Consumo_sig = new Consumo;

		Consumo_aux->Consumo_sig->Consumo_sig = nullptr;
		Consumo_aux->Consumo_sig->Consumo_ant = Consumo_aux;

		Consumo_aux = Consumo_aux->Consumo_sig;

		strcpy_s(Consumo_aux->Consumo_cliente, nuevo->Consumo_cliente);
		strcpy_s(Consumo_aux->Consumo_monto, nuevo->Consumo_monto);
		strcpy_s(Consumo_aux->Consumo_fecha, nuevo->Consumo_fecha);
		strcpy_s(Consumo_aux->Consumo_descuento, nuevo->Consumo_descuento);
		strcpy_s(Consumo_aux->Consumo_subtotal, nuevo->Consumo_subtotal);
		strcpy_s(Consumo_aux->Consumo_total, nuevo->Consumo_total);
		strcpy_s(Consumo_aux->Consumo_comercio, nuevo->Consumo_comercio);
		strcpy_s(Consumo_aux->Consumo_ID, nuevo->Consumo_ID);
		
	}
	MessageBox(0, "Consumo registrado", "AVISO", MB_OK);
}

void Escribir_Binario_Consumo()
{
	Consumo_aux = Consumo_inicio;

	ofstream escribirConsumo;
	escribirConsumo.open("C:\\Users\\PC\\Documents\\Datos binario\\Consumo.bin", ios::out | ios::binary | ios::trunc);

	if (escribirConsumo.is_open()) {
		while (Consumo_aux != nullptr)
		{
			escribirConsumo.write((char*)Consumo_aux, sizeof(Consumo));
			Consumo_aux = Consumo_aux->Consumo_sig;
		}

		escribirConsumo.close();
	}
	else
	{
		system("PAUSE");
	}
}

void Leer_Binario_Consumo()
{
	ifstream archivoConsumo;
	archivoConsumo.open("C:\\Users\\PC\\Documents\\Datos binario\\Consumo.bin", ios::in | ios::binary);


	if (archivoConsumo.is_open()) {

		Consumo* Consumo_leido = new Consumo;

		while (!archivoConsumo.read((char*)Consumo_leido, sizeof(Consumo)).eof()) {

			if (Consumo_inicio == nullptr) {
				Consumo_inicio = Consumo_leido;
				Consumo_inicio->Consumo_sig = nullptr;
				Consumo_inicio->Consumo_ant = nullptr;
				Consumo_aux = Consumo_inicio;
			}
			else {
				Consumo_aux->Consumo_sig = Consumo_leido;
				Consumo_aux->Consumo_sig->Consumo_ant = Consumo_aux;
				Consumo_aux = Consumo_aux->Consumo_sig;
				Consumo_aux->Consumo_sig = nullptr;
				//fin = aux;
			}

			Consumo_leido = new Consumo;
		}
		archivoConsumo.close();
		delete Consumo_leido;
	}
	else {
		system("PAUSE");
	}
}

void Agregar_Promocion(Promocion* nuevo) {


	if (Promocion_inicio == nullptr) {
		Promocion_inicio = new Promocion;
		Promocion_aux = Promocion_inicio;

		Promocion_aux->Promocion_sig = nullptr;
		Promocion_aux->Promocion_ant = nullptr;

		strcpy_s(Promocion_aux->Promocion_nombre, nuevo->Promocion_nombre);
		strcpy_s(Promocion_aux->Promocion_monto, nuevo->Promocion_monto);
		strcpy_s(Promocion_aux->Promocion_descuento, nuevo->Promocion_descuento);
		strcpy_s(Promocion_aux->Promocion_comercio, nuevo->Promocion_comercio);
		strcpy_s(Promocion_aux->Promocion_estatus, nuevo->Promocion_estatus);
		strcpy_s(Promocion_aux->Promocion_fecha, nuevo->Promocion_fecha);
		

	}
	else {
		Promocion_aux = Promocion_inicio;
		while (Promocion_aux->Promocion_sig != nullptr)
		{
			Promocion_aux = Promocion_aux->Promocion_sig;
		}

		Promocion_aux->Promocion_sig = new Promocion;

		Promocion_aux->Promocion_sig->Promocion_sig = nullptr;
		Promocion_aux->Promocion_sig->Promocion_ant = Promocion_aux;

		Promocion_aux = Promocion_aux->Promocion_sig;

		strcpy_s(Promocion_aux->Promocion_nombre, nuevo->Promocion_nombre);
		strcpy_s(Promocion_aux->Promocion_monto, nuevo->Promocion_monto);
		strcpy_s(Promocion_aux->Promocion_descuento, nuevo->Promocion_descuento);
		strcpy_s(Promocion_aux->Promocion_comercio, nuevo->Promocion_comercio);
		strcpy_s(Promocion_aux->Promocion_estatus, nuevo->Promocion_estatus);
		strcpy_s(Promocion_aux->Promocion_fecha, nuevo->Promocion_fecha);
		
		Promocion_aux->Promocion_sig = nullptr;
	}
	MessageBox(0, "Promocion registrada", "AVISO", MB_OK);
}

void Escribir_Binario_Promocion()
{
	Promocion_aux = Promocion_inicio;

	ofstream escribirPromocion;
	escribirPromocion.open("C:\\Users\\PC\\Documents\\Datos binario\\Promo.bin", ios::out | ios::binary | ios::trunc);

	if (escribirPromocion.is_open()) {
		while (Promocion_aux != nullptr)
		{
			escribirPromocion.write((char*)Promocion_aux, sizeof(Promocion));
			Promocion_aux = Promocion_aux->Promocion_sig;
		}

		escribirPromocion.close();
	}
	else
	{
		system("PAUSE");
	}
}

void Leer_Binario_Promocion()
{
	ifstream archivoPromocion;
	archivoPromocion.open("C:\\Users\\PC\\Documents\\Datos binario\\Promo.bin", ios::in | ios::binary);


	if (archivoPromocion.is_open()) {

		Promocion* Promocion_leido = new Promocion;

		while (!archivoPromocion.read((char*)Promocion_leido, sizeof(Promocion)).eof()) {

			if (Promocion_inicio == nullptr) {
				Promocion_inicio = Promocion_leido;
				Promocion_inicio->Promocion_sig = nullptr;
				Promocion_inicio->Promocion_ant = nullptr;
				Promocion_aux = Promocion_inicio;
			}
			else {
				Promocion_aux->Promocion_sig = Promocion_leido;
				Promocion_aux->Promocion_sig->Promocion_ant = Promocion_aux;
				Promocion_aux = Promocion_aux->Promocion_sig;
				Promocion_aux->Promocion_sig = nullptr;
				//fin = aux;
			}

			Promocion_leido = new Promocion;
		}
		archivoPromocion.close();
		delete Promocion_leido;
	}
	else {
		system("PAUSE");
	}
}

void Eliminar_Promocion(char nomPromocion[50]) {
	Promocion_aux = Promocion_inicio;

	if (Promocion_aux == nullptr)
	{
		MessageBox(0, "No hay promociones registradas", "AVISO", MB_OK | MB_ICONINFORMATION);
	}
	else {
		while (Promocion_aux != nullptr && strcmp(Promocion_aux->Promocion_nombre, nomPromocion) != 0) {

			Promocion_aux = Promocion_aux->Promocion_sig;
		}
		if (Promocion_aux == nullptr) {
			MessageBox(0, "Promocion no encontrada", "AVISO", MB_OK);
		}
		else if (Promocion_aux == Promocion_inicio) {	//El nodo es el primero 

			if (Promocion_aux->Promocion_sig == nullptr) {

				Promocion_inicio = nullptr;
				delete Promocion_aux;
				Promocion_aux = Promocion_inicio;
			}
			else {
				Promocion_inicio = Promocion_inicio->Promocion_sig;
				Promocion_aux->Promocion_sig->Promocion_ant = nullptr;
				delete Promocion_aux;
				Promocion_aux = Promocion_inicio;

			}

			MessageBox(0, "Promocion eliminada", "AVISO", MB_OK | MB_ICONINFORMATION);
		}
		else {

			if (Promocion_aux->Promocion_sig == nullptr) { //Si el nodo es el último
				Promocion_aux->Promocion_ant->Promocion_sig = nullptr;
				delete Promocion_aux;
				Promocion_aux = Promocion_inicio;
			}
			else {
				Promocion_aux->Promocion_sig->Promocion_ant = Promocion_aux->Promocion_ant;
				Promocion_aux->Promocion_ant->Promocion_sig = Promocion_aux->Promocion_sig;
				delete Promocion_aux;
				Promocion_aux = Promocion_inicio;
			}

			MessageBox(0, "Promoción eliminada", "AVISO", MB_OK | MB_ICONINFORMATION);

		}
	}

}