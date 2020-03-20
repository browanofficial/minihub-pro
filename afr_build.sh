#! /bin/sh

TOPDIR=$PWD
VENDOR='browan'
BOARD='tb-300'
COMPILER='xtensa-esp32'
BUILD_DIR=$TOPDIR/build
CMAKE=cmake
IDF_PY="$TOPDIR/vendors/$VENDOR/esp-idf/tools/idf.py"
CP2102_CTRL="$TOPDIR/vendors/$VENDOR/tools/cp210x_gpio"
ESP_UART_DETECT="$TOPDIR/vendors/$VENDOR/tools/esp_detect_uart.py"
BOARD_DIR="$TOPDIR/vendors/$VENDOR/boards/$BOARD"

act="$1"
opt="$2"
#force_opts="-DCMAKE_VERBOSE_MAKEFILE=ON"
#force_opts="-DAFR_ENABLE_TESTS=1"
esp_tty=""


show_usage () {
	cat <<EOF

Usage:
      $0 <cmake [-DCMake_Flags] | build | clean | rebuild>
      $0 <erase | flash | monitor>

      $0 reset          # reset MCU
      $0 download       # switch MCU to flash download mode

CMake_Flags Ex:
	-DCMAKE_VERBOSE_MAKEFILE=ON 
	-DCMAKE_BUILD_TYPE=debug|release

EOF
}


do_init () {
	if [ "$act" = "" ]; then
		show_usage
		exit 1
	fi

	$COMPILER-elf-gcc --version >/dev/null 2>&1 || {
		echo "ERROR >> please setup your toolchain: $COMPILER-elf-gcc"
		exit 1
	}

	if [ -f "$ESP_UART_DETECT" ]; then
		local tmp_uart=`$ESP_UART_DETECT`
		if [ -c "$tmp_uart" ]; then
			esp_tty="$tmp_uart"
		else
			esp_tty="/dev/ttyUSB1"
		fi
	fi
	echo "INFO >> use $esp_tty"
}

do_cmake () {
	local opt="$1"
	mkdir -p $BUILD_DIR
	$DO $CMAKE -DVENDOR=$VENDOR -DBOARD=$BOARD -DCOMPILER=$COMPILER $opt $force_opts -S $TOPDIR -B $BUILD_DIR
}

do_build () {
	if [ -d "$BUILD_DIR" ]; then
		$DO $CMAKE --build $BUILD_DIR
	else
		echo "WARN >> There is no $BUILD_DIR, do cmake first"
		exit 1
	fi
}

do_clean () {
	if [ -d "$BUILD_DIR" ]; then
		$DO cd $BUILD_DIR
		$DO make clean
		$DO cd $TOPDIR
	else
		echo "WARN >> There is no $BUILD_DIR, do cmake first"
		exit 1
	fi
}

do_idf_py () {
	local act="$1"

	case "$act" in
		monitor)
			$DO $IDF_PY $act -p $esp_tty -B $BUILD_DIR
			;;
		menuconfig)
			if [ -d "$BUILD_DIR" ]; then
				$DO $IDF_PY menuconfig
			else
				echo "WARN >> There is no $BUILD_DIR, do cmake first"
			fi
			;;
		flash|erase_flash)
			$CP2102_CTRL $esp_tty download
			$DO $IDF_PY $act -B $BUILD_DIR
			$CP2102_CTRL $esp_tty reset
			;;
		*)
			$DO $IDF_PY $act -B $BUILD_DIR
			;;
	esac
}

do_short_cmd () {
	local argv="$1"
	local argc=`echo -n $argv | wc -m`
	local short_cmd='';

	for i in `seq 0 $((${argc} - 1))`; do
		short_cmd="${argv:$i:1}"
		case $short_cmd in
			c)
				do_cmake "$opt"
				;;
			b)
				do_build
				;;
			e)
				do_idf_py "erase_flash"
				;;
			f)
				do_idf_py "flash"
				;;
			m)
				do_idf_py "monitor"
				;;
			*)
				echo "ERROR >> unknown short command: $short_cmd"
				show_usage
				exit 1
				;;
		esac
	done
}

do_action () {
	case $act in
		cmake|c)
			do_cmake "$opt"
			;;

		build|b)
			do_build
			;;

		clean)
			do_clean
			;;

		erase|e)
			do_idf_py "erase_flash"
			;;

		flash|f)
			do_idf_py "flash"
			;;

		monitor|m)
			do_idf_py "monitor"
			;;

		menuconfig)
			do_idf_py "menuconfig"
			;;

		reset)
			$CP2102_CTRL $esp_tty reset
			;;

		download)
			$CP2102_CTRL $esp_tty download
			;;

		*)
			do_short_cmd $act
			;;
	esac
}

do_main () {
	do_init
	do_action "$act" "$opt"
}

do_main
