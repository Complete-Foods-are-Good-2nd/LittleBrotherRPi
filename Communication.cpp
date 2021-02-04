#include "Communication.h"
// LB_SERIAL_DEVICE_DIR : socatでデバイスが作られるディレクトリ
// LB_SOCAT_DIR
// LB_AYAME_SIGNALINGKEY
// LB_AYAME_ROOM_ID
// LB_MOMO_DIR
// 環境変数として以上を設定する

Communication::Communication() {
    if (char *temp = getenv("LB_SERIAL_DEVICE_DIR")) {
        this->serial_device_dir = std::string(temp);
    } else {
        std::cerr << "Error: The environment variable: LB_SERIAL_DEVICE_DIR is "
                     "not set."
                  << std::endl;
        exit(1);
    }
    auto loopback_serial_device_names = generate_loopback_serial_devices();
    this->my_serial_device_name = loopback_serial_device_names.first;
    this->momo_serial_device_name = loopback_serial_device_names.second;
}

std::pair<std::string, std::string>
Communication::generate_loopback_serial_devices() {
    std::set<std::string> before_serial_device_names;
    std::set<std::string> after_serial_device_names;
    std::pair<std::string, std::string> loopback_serial_device_names;
    std::string socat_dir;
    if (char *temp = getenv("LB_SOCAT_DIR")) {
        socat_dir = std::string(temp);
    } else {
        std::cerr << "Error: The environment variable: LB_SOCAT_DIR is not set."
                  << std::endl;
        exit(1);
    }

    // SOCAT_CMD = sh -c '{socat_dir}socat -d -d pty,raw,echo=0 pty,raw,echo=0'
    // &
    std::string SOCAT_CMD = "sh -c '";
    SOCAT_CMD += socat_dir;
    SOCAT_CMD += "socat -d -d pty,raw,echo=0 pty,raw,echo=0' &";

    std::cout << SOCAT_CMD << std::endl;

    before_serial_device_names = get_serial_device_names();
    system(SOCAT_CMD.c_str());
    sleep(1);
    after_serial_device_names = get_serial_device_names();

    int virtual_serial_device_count = 0;
    for (const auto serial_device_name : after_serial_device_names) {
        if (!before_serial_device_names.count(serial_device_name)) {
            if (virtual_serial_device_count == 0) {
                loopback_serial_device_names.first = serial_device_name;
            } else if (virtual_serial_device_count == 1) {
                loopback_serial_device_names.second = serial_device_name;
            } else {
                std::cerr << "Error: Too many virtual-serial-devices, not 2."
                          << std::endl;
                exit(1);
            }
            ++virtual_serial_device_count;
        }
    }
    if (virtual_serial_device_count < 2) {
        std::cerr << "Error: Too few virtual-serial-devices, not 2."
                  << std::endl;
        exit(1);
    }
    return loopback_serial_device_names;
}

std::set<std::string> Communication::get_serial_device_names() {
    const char *serial_device_root_c = this->serial_device_dir.c_str();
    DIR *dir_stream;
    std::set<std::string> serial_device_names;
    struct dirent *dir_entry;

    dir_stream = opendir(serial_device_root_c);
    if (dir_stream == NULL) {
        std::cerr << "Error: Can't open dir: " << serial_device_dir
                  << std::endl;
        exit(1);
    }
    while ((dir_entry = readdir(dir_stream)) != NULL) {
        std::string serial_device_name(dir_entry->d_name,
                                       strlen(dir_entry->d_name));
        serial_device_names.insert(serial_device_name);
    }
    return serial_device_names;
}

void Communication::start_stream() {
    const std::string MOMO_CMD_OPTION_DEV =
        "--no-audio-device --no-video-device ayame "
        "wss://ayame-labo.shiguredo.jp/signaling";
    const std::string MOMO_CMD_OPTION_RPI =
        "--no-audio-device --resolution=VGA --force-i420 --hw-mjpeg-decoder "
        "true  ayame wss://ayame-labo.shiguredo.jp/signaling";
    std::string ayame_signalingkey;
    std::string ayame_room_id;
    std::string momo_dir;

    if (char *temp = getenv("LB_AYAME_SIGNALINGKEY")) {
        ayame_signalingkey = std::string(temp);
    } else {
        std::cerr << "Error: The environment variable: LB_AYAME_SIGNALINGKEY "
                     "is not set."
                  << std::endl;
        exit(1);
    }
    if (char *temp = getenv("LB_AYAME_ROOM_ID")) {
        ayame_room_id = std::string(temp);
    } else {
        std::cerr
            << "Error: The environment variable: LB_AYAME_ROOM_ID is not set."
            << std::endl;
        exit(1);
    }
    if (char *temp = getenv("LB_MOMO_DIR")) {
        momo_dir = std::string(temp);
    } else {
        std::cerr << "Error: The environment variable: LB_MOMO_DIR is not set."
                  << std::endl;
        exit(1);
    }

    // momo_cmd = sh -c '{momo_dir}momo --serial
    // {this->serial_device_dir}{this->momo_serial_device_name},9600
    // {MOMO_CMD_OPTION_*} {ayame_room_id} --signaling-key {ayame_signalingkey}'
    // &
    std::string MOMO_CMD = "sh -c '";
    MOMO_CMD += momo_dir;
    MOMO_CMD += "momo --serial";
    MOMO_CMD += " ";
    MOMO_CMD += this->serial_device_dir;
    MOMO_CMD += this->momo_serial_device_name;
    MOMO_CMD += ",9600";
    MOMO_CMD += " ";
    MOMO_CMD += MOMO_CMD_OPTION_RPI;
    MOMO_CMD += " ";
    MOMO_CMD += ayame_room_id;
    MOMO_CMD += " ";
    MOMO_CMD += "--signaling-key ";
    MOMO_CMD += " ";
    MOMO_CMD += ayame_signalingkey;
    MOMO_CMD += "' &";
    std::cout << MOMO_CMD << std::endl;
    system(MOMO_CMD.c_str());
}