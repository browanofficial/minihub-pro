
## Cloning

This repo uses [Git Submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules) to bring in dependent components.

Note: If you download the ZIP file provided by GitHub UI, you will not get the contents of the submodules. (The ZIP file is also not a valid git repository)

To clone using HTTPS:
```
git clone https://github.com/browanofficial/minihub-pro.git --recurse-submodules
```
Using SSH:
```
git clone git@github.com:browanofficial/minihub-pro.git --recurse-submodules
```

If you have downloaded the repo without using the `--recurse-submodules` argument, you need to run:
```
git submodule update --init --recursive
```


## The Hardware

MiniHub Pro use [Espressif ESP32 SoC](https://www.espressif.com/en/products/hardware/esp32/overview) as main chip.


## Getting Started

Access the **[Getting Started Guide](GettingStarted.md)** link for MiniHub Pro.

For more information on FreeRTOS, refer to the [Getting Started section of FreeRTOS webpage](https://aws.amazon.com/freertos).

For detailed documentation on FreeRTOS, refer to the [FreeRTOS User Guide](https://aws.amazon.com/documentation/freertos).
