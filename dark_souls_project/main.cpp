#include <iostream>
#include <libpq-fe.h>  // Librería para PostgreSQL

int main() {
    // Conectar a PostgreSQL con el usuario 'postgres' y la base de datos 'dark_souls_db'
    PGconn *conn = PQconnectdb("user=postgres password=tu_contraseña dbname=dark_souls_db");

    // Verificar si la conexión es exitosa
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Error de conexión a la base de datos: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);  // Cerrar la conexión si hubo error
        return 1;
    }

    std::cout << "Conexión exitosa a la base de datos!" << std::endl;

    PQfinish(conn);  // Cerrar la conexión
    return 0;
}