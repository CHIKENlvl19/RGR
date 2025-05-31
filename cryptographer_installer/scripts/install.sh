#!/bin/bash
set -e

# Проверка прав
if [ "$(id -u)" != "0" ]; then
  echo "Запустите установщик с правами root: sudo $0" >&2
  exit 1
fi

# Определение дистрибутива
if [ -f /etc/os-release ]; then
  . /etc/os-release
  OS=$ID
  if [[ "$OS" == "kali" || "$PRETTY_NAME" == *"Kali"* ]]; then
    OS="kali"
  fi
else
  OS=$(uname -s)
fi

# Установка зависимостей
echo "Установка системных зависимостей..."
case $OS in
  ubuntu|debian|pop|linuxmint|kali)
    apt-get update
    apt-get install -y build-essential libgmp-dev libssl-dev
    # Для Kali может потребоваться
    apt-get install -y patchelf || echo "Patchelf не установлен, продолжаем без него"
    ;;
  fedora|centos|rhel)
    yum groupinstall -y "Development Tools"
    yum install -y gmp-devel openssl-devel
    yum install -y patchelf || echo "Patchelf не установлен, продолжаем без него"
    ;;
  arch|manjaro)
    pacman -Sy --noconfirm base-devel gmp openssl
    pacman -Sy --noconfirm patchelf || echo "Patchelf не установлен, продолжаем без него"
    ;;
  *)
    echo "Неподдерживаемый дистрибутив: $OS" >&2
    echo "Установите вручную: build-essential, libgmp-dev, libssl-dev" >&2
    ;;
esac

# Установка файлов
echo "Установка программы..."
install -m 755 bin/cryptographer /usr/local/bin/cryptographer.bin

# Создаем обертку для запуска
echo "Создание обертки для запуска..."
cat > /usr/local/bin/cryptographer <<EOL
#!/bin/bash
export LD_LIBRARY_PATH="/usr/local/lib:\$LD_LIBRARY_PATH"
exec /usr/local/bin/cryptographer.bin "\$@"
EOL
chmod +x /usr/local/bin/cryptographer

echo "Установка библиотек..."
install -m 644 libs/*.so /usr/local/lib/

# Используем patchelf если доступен
if command -v patchelf &> /dev/null; then
  echo "Исправление путей к библиотекам с помощью patchelf..."
  patchelf --set-rpath /usr/local/lib /usr/local/bin/cryptographer.bin
else
  echo "Patchelf не установлен, используем обертку"
fi

# Обновление кэша библиотек
ldconfig

echo "Проверка установки библиотек..."
for lib in elgamal vernam viginer; do
  if [ ! -f "/usr/local/lib/lib${lib}.so" ]; then
    echo "ОШИБКА: Библиотека lib${lib}.so не найдена в /usr/local/lib!" >&2
    exit 1
  fi
  if ! ldconfig -p | grep -q "lib${lib}.so"; then
    echo "ПРЕДУПРЕЖДЕНИЕ: Библиотека lib${lib}.so не в кэше ldconfig" >&2
  fi
done

echo "Установка заголовочных файлов..."
mkdir -p /usr/local/include/cryptographer
cp -r include/* /usr/local/include/cryptographer/

echo "Установка подготовленных файлов..."
mkdir -p /usr/local/share/cryptographer/prepared_files
cp -r prepared_files/* /usr/local/share/cryptographer/prepared_files/

echo "Установка скриптов управления..."
mkdir -p /usr/local/share/cryptographer/scripts
cp scripts/*.sh /usr/local/share/cryptographer/scripts/
chmod +x /usr/local/share/cryptographer/scripts/*.sh

# Создание конфигурации
if [ ! -f /etc/cryptographer.conf ]; then
  echo "Создание конфигурационного файла..."
  cat > /etc/cryptographer.conf <<EOL
# Конфигурация Cryptographer
library_path = /usr/local/lib
data_path = /usr/local/share/cryptographer/prepared_files
scripts_path = /usr/local/share/cryptographer/scripts
EOL
  chmod 644 /etc/cryptographer.conf
fi

# Проверка установки
echo "Проверка установки..."
if /usr/local/bin/cryptographer --version; then
  echo "Установка завершена успешно!"
  echo "Для использования: cryptographer"
else
  echo "Ошибка при проверке установки!" >&2
  
  # Расширенная диагностика
  echo "=== Диагностическая информация ==="
  echo "Библиотеки в /usr/local/lib:"
  ls -l /usr/local/lib/libelgamal* /usr/local/lib/libvernam* /usr/local/lib/libviginer*
  
  echo "LD_LIBRARY_PATH: $LD_LIBRARY_PATH"
  
  echo "ldconfig cache:"
  ldconfig -p | grep -e elgamal -e vernam -e viginer
  
  echo "Содержимое /etc/ld.so.conf.d:"
  ls -l /etc/ld.so.conf.d/
  
  echo "Проверка бинарника:"
  ldd /usr/local/bin/cryptographer.bin
  
  exit 1
fi