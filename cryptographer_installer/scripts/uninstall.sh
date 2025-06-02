#!/bin/bash

if [ "$(id -u)" != "0" ]; then
    echo "Требуются права root! Запустите: sudo $0" >&2
    exit 1
fi

echo "=============================================="
echo "          Удаление Cryptographer"
echo "=============================================="
echo "Будут удалены следующие компоненты:"
echo "1. Исполняемый файл: /usr/local/bin/cryptographer"
echo "2. Библиотеки: /usr/local/lib/lib{elgamal,vernam,viginer}.so"
echo "3. Ресурсы: /usr/local/share/cryptographer"
echo "4. Конфигурация: /etc/cryptographer.conf"
echo "5. Скрипты управления"
echo "=============================================="

read -p "Вы уверены, что хотите удалить программу? [y/N] " response
if [[ ! "$response" =~ ^[Yy] ]]; then
    echo "Удаление отменено."
    exit 0
fi

echo "Начало удаления..."
rm -f /usr/local/bin/cryptographer
rm -f /usr/local/bin/cryptographer.bin
rm -f /usr/local/bin/cryptographer-uninstall
rm -f /usr/local/lib/libelgamal.so
rm -f /usr/local/lib/libvernam.so
rm -f /usr/local/lib/libviginer.so
rm -rf /usr/local/include/cryptographer
rm -rf /usr/local/share/cryptographer
rm -f /etc/cryptographer.conf

echo "Обновление кэша библиотек..."
ldconfig

echo "=============================================="
echo "        Cryptographer успешно удален!"
echo "=============================================="