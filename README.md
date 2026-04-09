# TypeApe

A fast, minimal terminal typing speed test — inspired by MonkeyType.

## Features
- Large word dictionary, streamed randomly (zero memory load)
- Real-time WPM, raw WPM, accuracy, consistency stats
- 3-row scrolling display with async word prefetch
- Timer: 15s / 30s / 45s / 60s / custom (inline input box)
- Live terminal resize support
- Blinking cursor on idle, steady while typing
- Quick restart with Enter, graceful exit with ESC

## Install

### Arch Linux (AUR)
```bash
yay -S typeape
```

### Homebrew (macOS / Linux)
```bash
brew tap realSUDO/typeape
brew install typeape
```

### Script — Debian, Ubuntu, Fedora, any distro
```bash
git clone https://github.com/realSUDO/typeape.git
cd typeape
bash scripts/install.sh
```

### Make
```bash
git clone https://github.com/realSUDO/typeape.git
cd typeape
make
sudo make install
```

### CMake
```bash
git clone https://github.com/realSUDO/typeape.git
cd typeape
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
sudo cmake --install build
```

## Usage
```
typeape
```
| Key | Action |
|-----|--------|
| ↑↓ | select timer |
| Enter | confirm / quick restart |
| ESC | exit |

## Stats
| Stat | Description |
|------|-------------|
| wpm | correct chars (incl. spaces) / 5 / minutes |
| raw | all typed chars / 5 / minutes |
| acc | correct keypresses % |
| chars | correct / incorrect char count |
| consist | CV of raw wpm intervals → 0–100% |

## License
GPL-3.0 — see [LICENSE](LICENSE)

## by [github.com/realSUDO](https://github.com/realSUDO)
