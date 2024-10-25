#pragma once

namespace esphome {
namespace rda5820 {

static const float FREQ_MIN = 50;
static const float FREQ_MAX = 108;

enum class ClkMode : uint16_t {
  CLK_MODE_32K768,
  CLK_MODE_12M,
  CLK_MODE_24M,
  CLK_MODE_13M,
  CLK_MODE_26M,
  CLK_MODE_19M2,
  CLK_MODE_38M4,
  LAST,
};

enum class SkMode : uint16_t {
  SKMODE_WRAP,
  SKMODE_STOP,
  LAST,
};

enum class ChannelSpacing : uint16_t {
  SPACE_100K,
  SPACE_200K,
  SPACE_50K,
  SPACE_25K,
  LAST,
};

enum class BandSelect : uint16_t {
  BAND_87_108,
  BAND_76_91,
  BAND_76_108,
  BAND_65_76_OR_50_65,  // depends on REG_07.MODE_50M_65M
  LAST,
};

enum class Gpio1Control : uint16_t {
  GPIO_HIGH_Z,
  GPIO_RESERVED,
  GPIO_LOW,
  GPIO_HIGH,
  LAST,
};

enum class Gpio2Control : uint16_t {
  GPIO_HIGH_Z,
  GPIO_INTERRUPT,
  GPIO_LOW,
  GPIO_HIGH,
  LAST,
};

enum class Gpio3Control : uint16_t {
  GPIO_HIGH_Z,
  GPIO_STEREO_INDICATOR,
  GPIO_LOW,
  GPIO_HIGH,
  LAST,
};

enum class DeEmphasis : uint16_t {
  DE_75US,
  DE_50US,
  LAST,
};

enum class LnaInputPort : uint16_t {
  LNA_PORT_SEL_NO_INPUT,
  LNA_PORT_SEL_LNAN,
  LNA_PORT_SEL_LNAP,
  LNA_PORT_SEL_DUAL_PORT_INPUT,
  LAST,
};

enum class SeekMode : uint16_t {
  SEEK_MODE_DEFAULT,  // this is the default, but no description(?)
  SEEK_MODE_AND,      // adjacent seek process && noise condition
  SEEK_MODE_OR,       // adjacent seek process || noise condition
  LAST,
};

enum class InterruptMode : uint16_t {
  INT_MODE_5MS,
  INT_MODE_UNTIL_STATUS_READ,
  LAST,
};

enum class I2sWordSelect : uint16_t {
  I2S_SW_CNT_8KBPS,
  I2S_SW_CNT_11K025BPS,
  I2S_SW_CNT_12KBPS,
  I2S_SW_CNT_16KBPS,
  I2S_SW_CNT_22K05BPS,
  I2S_SW_CNT_24KBPS,
  I2S_SW_CNT_32KBPS,
  I2S_SW_CNT_44K1BPS,
  I2S_SW_CNT_48KBPS,
  LAST,
};

enum class I2sMode : uint16_t {
  I2S_MODE_MASTER,
  I2S_MODE_SLAVE,
  LAST,
};

enum class BlockErrorsLevel : uint16_t {
  BLER_0,
  BLER_1_2,
  BLER_3_5,
  BLER_6_PLUS,
  LAST,
};

enum class WorkMode : uint16_t {
  WORK_MODE_FM_RECEIVE,
  WORK_MODE_FM_TRANSMIT,
  WORK_MODE_AUDIO_AMPLIFY,
  WORK_MODE_CODEC,
  WORK_MODE_ADC,
  LAST,
};

enum class PaGain : uint16_t {
  // is this preset, or can we use any other 6-bit value?
  PA_GAIN_N32DB = 0,  // -32dBm
  PA_GAIN_N3DB = 19,  // -3dBm
  PA_GAIN_0DB = 39,   // 0dBm
  PA_GAIN_P3DB = 63,  // 3dBm
  LAST,
};

enum class PgaGain : uint16_t {
  PGA_GAIN_1200,  // mV
  PGA_GAIN_600,
  PGA_GAIN_300,
  PGA_GAIN_150,
  PGA_GAIN_75,
  PGA_GAIN_37,
  PGA_GAIN_18,
  PGA_GAIN_9,
  LAST,
};

struct RDA5820State {
  union {
    uint16_t REG_00;
    struct {
      uint16_t CHIP_ID;
    };
  };
  uint16_t REG_01;
  union {
    uint16_t REG_02;
    struct {
      uint16_t ENABLE : 1;
      uint16_t SOFT_RESET : 1;
      uint16_t : 1;
      uint16_t RDS_EN : 1;
      uint16_t CLK_MODE : 3;  // ClkMode
      uint16_t SKMODE : 1;    // SkMode
      uint16_t SEEK : 1;
      uint16_t SEEKUP : 1;
      uint16_t RCLK_DIRECT_INPUT_MODE : 1;
      uint16_t RCLK_NON_CALIBRATE_MODE : 1;
      uint16_t BASS : 1;   // Bass boost
      uint16_t MONO : 1;   // Force mono
      uint16_t DMUTE : 1;  // Disable mute
      uint16_t DHIZ : 1;   // Disable High-Z impediance
    };
  };
  union {
    uint16_t REG_03;
    struct {
      uint16_t SPACE : 2;        // ChannelSpacing
      uint16_t BAND : 2;         // BandSelect
      uint16_t TUNE : 1;         // Tune operation begins when the TUNE bit is set high, reset when complete
      uint16_t DIRECT_MODE : 1;  // Only used when test
      uint16_t CHAN : 10;        // Channel Spacing (kHz) x CHAN + BAND START (MHz)
    };
  };
  union {
    uint16_t REG_04;
    struct {
      uint16_t GPIO1 : 2;        // Gpio1Control
      uint16_t GPIO2 : 2;        // Gpio2Control
      uint16_t GPIO3 : 2;        // Gpio3Control
      uint16_t I2S_ENABLED : 1;  // SCK, WS, SD from GPIO3, GPIO1, GPIO2 as master, sample rate is 42Kbps
      uint16_t AFCD : 1;         // Disable AFC
      uint16_t SOFTMUTE_EN : 1;
      uint16_t RDS_FIFO_CLR : 1;  // Clear RDS fifo
      uint16_t DE : 1;            // DeEmphasis
      uint16_t RDS_FIFO_EN : 1;   //  RDS fifo mode enable
      uint16_t RBDS : 1;          // RBDS mode enable
      uint16_t STCIEN : 1;        // Seek/Tune complete interrupt on GPIO2
      uint16_t RDSIEN : 1;        // RDS ready interrupt on GPIO2
    };
  };
  union {
    uint16_t REG_05;
    struct {
      uint16_t VOLUME : 4;  // Volume scale is logarithmic
      uint16_t : 2;
      uint16_t LNA_PORT_SEL : 2;  // LnaInputPort
      uint16_t SEEKTH : 4;        // Seek SNR threshold value, 79 - SEEKTH (dB)
      uint16_t : 1;
      uint16_t SEEK_MODE : 2;  // SeekMode
      uint16_t INT_MODE : 1;   // InterruptMode
    };
  };
  union {
    uint16_t REG_06;  // I2S only
    struct {
      uint16_t R_DELY : 1;       // R channel data delay 1T
      uint16_t L_DELY : 1;       // L channel data delay 1T
      uint16_t SCLK_O_EDGE : 1;  // invert sclk output when as master
      uint16_t SW_O_EDGE : 1;    // invert ws output when as master
      uint16_t I2S_SW_CNT : 4;   // I2sWordSelect, only valid in master mode
      uint16_t WS_I_EDGE : 1;    // invert ws internally
      uint16_t DATA_SIGNED : 1;  // signed 16-bit audio
      uint16_t SCLK_I_EDGE : 1;  // invert sclk internally
      uint16_t SW_LR : 1;        // WS relation to l/r channel, 0 => RL, 1 => LR
      uint16_t I2S_MODE : 1;     // I2sMode
      uint16_t : 3;
    };
  };
  union {
    uint16_t REG_07;
    struct {
      uint16_t FREQ_MODE : 1;  // Freq setting changed, 76000(or 87000) kHz + freq_direct (08H) kHz (?)
      uint16_t SOFTBLEND_EN : 1;
      uint16_t SEEK_TH_OLD : 6;   // Seek threshold for old seek mode, valid when SeekMode::SEEK_MODE_AND
      uint16_t MODE_50M_65M : 1;  // When BAND == BAND_65_76_OR_50_65
      uint16_t TH_SOFTBLEND : 5;  // If (127-noise_db2)>4*th, turn soft blend off
      uint16_t : 1;
    };
  };
  uint16_t REG_08;
  uint16_t REG_09;
  union {
    uint16_t REG_0A;
    struct {
      uint16_t READCHAN : 10;  // Updated after a tune or seek operation
      uint16_t ST : 1;         // Stereo indicator, available on GPIO3 by setting GPIO3=01
      uint16_t BLK_E : 1;      // Block E has been found
      uint16_t RDSS : 1;       // RDS decoder synchronized (in RDS Verbose mode)
      uint16_t SF : 1;         // Seek fail, RSSI level lower than SEEKTH
      uint16_t STC : 1;        // Seek/Tune complete
      uint16_t RDSR : 1;       // New RDS/RBDS group ready
    };
  };
  union {
    uint16_t REG_0B;
    struct {
      uint16_t BLERB : 2;   // BlockErrorsLevel
      uint16_t BLERA : 2;   // BlockErrorsLevel
      uint16_t ABCD_E : 1;  // => see RDSA
      uint16_t : 2;
      uint16_t FM_READY : 1;
      uint16_t FM_TRUE : 1;  // The current channel is a station
      uint16_t RSSI : 7;
    };
  };
  union {
    uint16_t REG_0C;
    struct {
      uint16_t RDSA;  // BLOCK A (in RDS mode) or BLOCK E (in RBDS mode when ABCD_E flag is 1)
    };
  };
  union {
    uint16_t REG_0D;
    struct {
      uint16_t RDSB;
    };
  };
  union {
    uint16_t REG_0E;
    struct {
      uint16_t RDSC;
    };
  };
  union {
    uint16_t REG_0F;
    struct {
      uint16_t RDSD;
    };
  };
  uint16_t RESERVED1[0x30];  // skip 0x10-0x3F
  union {
    uint16_t REG_40;
    struct {
      uint16_t WORK_MODE : 4;  // WorkMode
      uint16_t : 12;
    };
  };
  union {
    uint16_t REG_41;
    struct {
      uint16_t PA_GAIN : 6;  // TXPA Gain Bit, PaGain
      uint16_t PA_IBIT : 3;  // TXPA Bias Current
      uint16_t PA_VCOM : 3;  // TXPA Common Voltage
      uint16_t : 4;
    };
  };
  uint16_t RESERVED2[0x25];  // skip 0x42-0x66
  union {
    uint16_t REG_67;
    struct {
      uint16_t RDS_DEV : 8;    // FM Transmit RDS Signal Modulate Parameter
      uint16_t PILOT_DEV : 8;  // FM Transmit Pilot Tone Modulate Parameter
    };
  };
  union {
    uint16_t REG_68;
    struct {
      uint16_t AUDIO_DEV : 8;  // FM Transmit Audio Signal Modulate Parameter
      uint16_t ADC_GAIN : 2;   // FM Transmit ADC Gain Bit
      uint16_t PGA_GAIN : 3;   // FM Transmit PGA Gain Bit, PgaGain
      uint16_t : 3;
    };
  };
};

}  // namespace rda5820
}  // namespace esphome
