// ConexionBD.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;


int main() {
    SQLHENV hEnv;
    SQLHDBC hDbc;
    SQLRETURN ret;

    // Asignar un gestor de entorno
    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    ret = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

    // Asignar un gestor de conexión
    ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

    // Conectarse a la base de datos
    ret = SQLConnect(hDbc, (SQLWCHAR*)L"sqlserver", SQL_NTS, (SQLWCHAR*)L"Username", SQL_NTS, (SQLWCHAR*)L"Password", SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        cout << "Conectado a la base de datos exitosamente." << endl;

        //Ejemplo de ejecucion de una consulta
        SQLHSTMT hStmt;
        ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

        //Ejemplo de consulta SELECT
        ret = SQLExecDirect(hStmt, (SQLWCHAR*)L"SELECT * FROM Empleados", SQL_NTS);
        if (ret == SQL_SUCCESS || ret == SQL_ROW_SUCCESS_WITH_INFO) {
            int ID;
            SQLCHAR nombre[255], apellidoPaterno[255], apellidoMaterno[255], fechaNacimiento[255], rfc[255], centroTrabajo[255], puesto[255], descPuesto[255], directivo[5];
            
            // Imprimir encabezados de tabla
            cout << "+-----+------------------------+-------------------+--------------+-------------------------------+------------------------+-------------------------------+-----------+" << endl;
            cout << "| ID  | Nombre Completo        | Fecha Nacimiento  | RFC          | Centro de Trabajo             | Puesto                 |Descripcion del Puesto         | Directivo |" << endl;
            cout << "+-----+------------------------+-------------------+--------------+-------------------------------+------------------------+-------------------------------+-----------+" << endl;


            // Extraer e imprimir datos de la consulta
            while (SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_LONG, &ID, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_CHAR, nombre, sizeof(nombre), NULL);
                SQLGetData(hStmt, 3, SQL_C_CHAR, apellidoPaterno, sizeof(apellidoPaterno), NULL);
                SQLGetData(hStmt, 4, SQL_C_CHAR, apellidoMaterno, sizeof(apellidoMaterno), NULL);
                SQLGetData(hStmt, 5, SQL_C_CHAR, fechaNacimiento, sizeof(fechaNacimiento), NULL);
                SQLGetData(hStmt, 6, SQL_C_CHAR, rfc, sizeof(rfc), NULL);
                SQLGetData(hStmt, 7, SQL_C_CHAR, centroTrabajo, sizeof(centroTrabajo), NULL);
                SQLGetData(hStmt, 8, SQL_C_CHAR, puesto, sizeof(puesto), NULL);
                SQLGetData(hStmt, 9, SQL_C_CHAR, descPuesto, sizeof(descPuesto), NULL);
                SQLGetData(hStmt, 10, SQL_C_CHAR, directivo, sizeof(directivo), NULL);

                string nombreCompleto = string((char*)nombre) + " " + string((char*)apellidoPaterno) + " " + string((char*)apellidoMaterno);
                string dir = string((char*)directivo);
                string respuesta;

                if (dir == "1") {
                    respuesta = "SI";
                }
                else
                {
                    respuesta = "NO";
                }


                cout << "| " << left << setw(4) << ID
                    << "| " << left << setw(23) << nombreCompleto
                    << "| " << left << setw(18) << fechaNacimiento
                    << "| " << left << setw(13) << rfc
                    << "| " << left << setw(30) << centroTrabajo
                    << "| " << left << setw(23) << puesto
                    << "| " << left << setw(30) << descPuesto
                    << "| " << left << setw(10) << respuesta
                    << "|" << endl;
            }
            cout << "+-----+------------------------+-------------------+--------------+-------------------------------+------------------------+-------------------------------+-----------+" << endl;
        }

        // Liberar el manejador de conexion
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    }
    else {
        cout << "Fallo la conexion a la base de datos" << endl;
    }

    // Desconectar y liberar gestores de entorno
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    return 0;
}
