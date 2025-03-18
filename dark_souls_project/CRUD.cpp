#include <iostream>
#include <libpq-fe.h>  // Librería para trabajar con PostgreSQL
#include <string>

using namespace std;

// Estructura para representar un boss
struct Boss {
    int id;                 // ID único para cada boss
    string name;            // Nombre del boss
    string location;        // Ubicación del boss en el juego
    int health;             // Salud del boss
    int souls;              // Almas que da al derrotarlo
    string importantItems;  // Artículos importantes asociados al boss
    string status;          // Estado de vida del boss: "alive" o "dead"
};

// Función para conectar a la base de datos PostgreSQL
PGconn* connectDB() {
    PGconn* conn = PQconnectdb("user=postgres password=tu_contraseña dbname=dark_souls_db");
    if (PQstatus(conn) != CONNECTION_OK) {
        // Si la conexión falla, mostramos el error
        cerr << "Error de conexión: " << PQerrorMessage(conn) << endl;
        return nullptr;  // Retornamos nullptr si la conexión falla
    }
    return conn;
}

// Función para insertar un boss en la base de datos
void insertBoss(PGconn* conn, const Boss& boss) {
    // Consulta SQL para insertar un nuevo registro en la tabla 'bosses'
    string query = "INSERT INTO bosses (name, location, health, souls, important_items, status) VALUES ('" + boss.name + "', '" + boss.location + "', " + to_string(boss.health) + ", " + to_string(boss.souls) + ", '" + boss.importantItems + "', '" + boss.status + "');";
    
    // Ejecutamos la consulta
    PGresult* res = PQexec(conn, query.c_str());

    // Comprobamos si la consulta fue exitosa
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Error al insertar boss: " << PQerrorMessage(conn) << endl;
    } else {
        cout << "Boss insertado correctamente." << endl;
    }

    PQclear(res);  // Limpiamos el resultado de la consulta
}

// Función para leer todos los bosses de la base de datos
void readAllBosses(PGconn* conn) {
    PGresult* res = PQexec(conn, "SELECT * FROM bosses;");
    
    // Comprobamos si la consulta fue exitosa
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "Error al leer los bosses: " << PQerrorMessage(conn) << endl;
        return;
    }
    
    // Recorremos todos los registros y mostramos la información de cada boss
    int rows = PQntuples(res);  // Número de registros devueltos
    for (int i = 0; i < rows; ++i) {
        cout << "ID: " << PQgetvalue(res, i, 0) << ", ";
        cout << "Nombre: " << PQgetvalue(res, i, 1) << ", ";
        cout << "Ubicación: " << PQgetvalue(res, i, 2) << ", ";
        cout << "Salud: " << PQgetvalue(res, i, 3) << ", ";
        cout << "Almas: " << PQgetvalue(res, i, 4) << ", ";
        cout << "Artículos: " << PQgetvalue(res, i, 5) << ", ";
        cout << "Estado: " << PQgetvalue(res, i, 6) << endl;
    }

    PQclear(res);  // Limpiamos el resultado de la consulta
}

// Función para actualizar un boss en la base de datos
void updateBoss(PGconn* conn, int id, const Boss& updatedBoss) {
    string query = "UPDATE bosses SET name = '" + updatedBoss.name + "', location = '" + updatedBoss.location + "', health = " + to_string(updatedBoss.health) + ", souls = " + to_string(updatedBoss.souls) + ", important_items = '" + updatedBoss.importantItems + "', status = '" + updatedBoss.status + "' WHERE id = " + to_string(id) + ";";
    
    // Ejecutamos la consulta
    PGresult* res = PQexec(conn, query.c_str());
    
    // Comprobamos si la consulta fue exitosa
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Error al actualizar boss: " << PQerrorMessage(conn) << endl;
    } else {
        cout << "Boss actualizado correctamente." << endl;
    }

    PQclear(res);  // Limpiamos el resultado de la consulta
}

// Función para eliminar un boss de la base de datos
void deleteBoss(PGconn* conn, int id) {
    string query = "DELETE FROM bosses WHERE id = " + to_string(id) + ";";
    
    // Ejecutamos la consulta
    PGresult* res = PQexec(conn, query.c_str());

    // Comprobamos si la consulta fue exitosa
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Error al eliminar boss: " << PQerrorMessage(conn) << endl;
    } else {
        cout << "Boss eliminado correctamente." << endl;
    }

    PQclear(res);  // Limpiamos el resultado de la consulta
}

// Función para mostrar el menú y manejar la entrada del usuario
void showMenu() {
    cout << "Menu de opciones:" << endl;
    cout << "1. Insertar nuevo boss" << endl;
    cout << "2. Leer todos los bosses" << endl;
    cout << "3. Actualizar un boss" << endl;
    cout << "4. Eliminar un boss" << endl;
    cout << "5. Salir" << endl;
}

// Función principal donde ejecutamos el programa
int main() {
    // Conectar a la base de datos
    PGconn* conn = connectDB();
    if (!conn) {
        return 1;  // Si la conexión falla, terminamos el programa
    }

    int choice;
    do {
        showMenu();
        cout << "Seleccione una opción: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                // Insertar un nuevo boss
                Boss newBoss = {"Gwyn, Lord of Cinder", "Kiln of the First Flame", 4000, 50000, "Soul of Gwyn, Cinders of Gwyn", "dead"};
                insertBoss(conn, newBoss);
                break;
            }
            case 2:
                // Leer todos los bosses
                readAllBosses(conn);
                break;
            case 3: {
                // Actualizar un boss
                Boss updatedBoss = {"Artorias the Abysswalker", "The Abyss", 4500, 20000, "Soul of Artorias, Greatsword of Artorias", "dead"};
                updateBoss(conn, 15, updatedBoss);  // Aquí usas el ID del boss que quieres actualizar
                break;
            }
            case 4: {
                // Eliminar un boss
                deleteBoss(conn, 14);  // Aquí usas el ID del boss que quieres eliminar
                break;
            }
            case 5:
                cout << "Saliendo del programa..." << endl;
                break;
            default:
                cout << "Opción no válida. Intente nuevamente." << endl;
        }
    } while (choice != 5);

    PQfinish(conn);  // Cerramos la conexión a la base de datos
    return 0;
}


