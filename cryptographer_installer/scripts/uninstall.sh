#!/bin/bash

if [ "$(id -u)" != "0" ]; then
  echo "Запустите скрипт с правами root: sudo $0" >&2
  exit 1
fi

echo "Удаление Cryptographer..."
rm -f /usr/local/bin/cryptographer
rm -f /usr/local/lib/libelgamal.so
rm -f /usr/local/lib/libvernam.so
rm -f /usr/local/lib/libviginer.so
rm -rf /usr/local/include/cryptographer
rm -rf /usr/local/share/cryptographer

echo "Удаление конфигурации..."
rm -f /etc/cryptographer.conf

echo "Обновление кэша библиотек..."
ldconfig

echo "Cryptographer успешно удален!"