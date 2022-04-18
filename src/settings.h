#pragma once
#include <Arduino.h>

namespace Settings {
constexpr int NumberOfLeds = 98;
constexpr char DiPin = 0;
constexpr char PhotoResPin = 2;
// время , через которое лента выключится при пропадаании сигнала
constexpr long OffTime = 10 * 10e3;
// лимит по току в миллиамперах, автоматически управляет яркостью (0- выключить
// лимит)
constexpr int CurrentLimit = 2000;
// проверка цветов при запуске
constexpr boolean StartBlink = false;
// автоматическая подстройка яркости от уровня внешнего освещения
constexpr boolean AutoBright = true;
// константа усиления от внешнего света (0 - 1023) чем МЕНЬШЕ константа, тем
// "резче" будет прибавляться яркость
constexpr int BrightConstant = 500;
// коэффициент фильтра (0.0 - 1.0), чем больше - тем медленнее меняется яркость
constexpr int Coef = 0.9;
}  // namespace Settings