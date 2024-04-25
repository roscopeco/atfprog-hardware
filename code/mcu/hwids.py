Import("env")

board_config = env.BoardConfig()

board_config.update("build.hwids", [
    ["0x04D8", "0xE5C7"]
])

board_config.update("board_build.usb_product", "Little ATF150x Programmer")
board_config.update("board_build.usb_manufacturer", "The Really Old-School Company Ltd")