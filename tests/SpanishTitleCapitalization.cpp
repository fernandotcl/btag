/*
 * This file is part of btag.
 *
 * © 2011 Fernando Tarlá Cardoso Lemos
 *
 * Refer to the LICENSE file for licensing information.
 *
 */

#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include <iostream>

#include "SpanishTitleLocalizationHandler.h"
#include "TitleCapitalizationFilter.h"

/* This was initialized with a list of best-sellers found on
 * Wikipedia (though only those that we should handle well).
 * Feel free to extend it with whatever content. */
static const wchar_t *correct[] = {
    L"Historia de Dos Ciudades",
    L"El Hobbit",
    L"Sueño en el Pabellón Rojo",
    L"Triple Representatividad",
    L"Diez Negritos",
    L"El León, la Bruja y el Armario",
    L"Ella",
    L"El Principito",
    L"El Guardián Entre el Centeno",
    L"El Alquimista",
    /* 10 */
    L"Heidi",
    L"El Libro del Sentido del Común del Cuidado de Bebés y Niños",
    L"El Nombre de la Rosa",
    L"Ángeles y Demonios",
    L"Así se Templó el Acero",
    L"Guerra y Paz",
    L"Usted Puede Sanar Su Vida",
    L"Matar a un Ruiseñor",
    L"El Valle de las Muñecas",
    L"Lo que el Viento se Llevó",
    /* 20 */
    L"Cien Años de Soledad",
    L"Una Vida con Propósito",
    L"El Pájaro Espino",
    L"Piense y Hágase Rico",
    L"Los Hombres que No Amaban a las Mujeres",
    L"La Oruguita Glotona",
    L"¿Quién se Ha Llevado Mi Queso?",
    L"El Viento en los Sauces",
    L"1984",
    L"La Prostituta Feliz",
    /* 30 */
    L"Tiburón",
    L"Siempre te Querré",
    L"La Habitación de las Mujeres",
    L"Qué Esperar Cuando se Está Esperando",
    L"Donde Viven los Monstruos",
    L"El Secreto",
    L"Miedo a Volar",
    L"Adivina Cuánto te Quiero",
    L"Los Siete Hábitos de las Personas Altamente Efectivas",
    L"Cómo Ganar Amigos e Influir Sobre las Personas",
    /* 40 */
    L"El Perfume: Historia de un Asesino",
    L"El Hombre que Susurraba al Oído de los Caballos",
    L"La Sombra del Viento",
    L"La Cabaña",
    L"Guía del Autoestopista Galáctico",
    L"Martes con Mi Viejo Profesor",
    L"Donde el Corazón te Lleve",
    L"Rebeldes",
    L"Charlie y la Fábrica de Chocolate",
    L"La Peste",
    /* 50 */
    L"Indigno de Ser Humano",
    L"El Mono Desnudo",
    L"Todo se Desmorona",
    L"El Profeta",
    L"El Exorcista",
    L"El Grúfalo",
    L"Trampa-22",
    L"La Isla de las Tormentas",
    L"Historia del Tiempo",
    L"El Gato con Sombrero",
    /* 60 */
    L"Desde Mi Cielo",
    L"Cisnes Salvajes",
    L"La Noche",
    L"Cometas en el Cielo",
    L"La Mujer Total",
    L"Historia del Futuro, la Sociedad del Conocimiento",
    L"¿De Qué Color Es Su Paracaídas?"
};

int main(int argc, char **argv) {
    // Force an UTF-8 locale
    std::ios_base::sync_with_stdio(false);
    std::locale locale(std::locale::classic(), "C.UTF-8", std::locale::ctype);
    std::locale::global(locale);

    // Set up the capitalization filter
    SpanishTitleLocalizationHandler handler;
    TitleCapitalizationFilter filter;
    filter.set_localization_handler(&handler);

    int errors = 0;
    for (size_t i = 0; i < sizeof(correct) / sizeof(const wchar_t *); ++i) {
        // Get lowercase and uppercase versions of the string
        std::wstring correctStr(correct[i]);
        std::wstring lowerStr(boost::algorithm::to_lower_copy(correctStr));
        std::wstring upperStr(boost::algorithm::to_upper_copy(correctStr));

        // Apply the localization handler
        std::wstring lowerFiltered(filter.filter(lowerStr));
        std::wstring upperFiltered(filter.filter(upperStr));

        // Make sure they match
        if (lowerFiltered != correctStr) {
            std::cerr << "Incorrect conversion from lowercase for index " << i + 1 << std::endl;
            ++errors;
        }
        if (upperFiltered != correctStr) {
            std::cerr << "Incorrect conversion from uppercase for index " << i + 1 << std::endl;
            ++errors;
        }
    }

    return errors == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
