# Localization mod for Death Stranding 2

### Installation

1. Download the latest release from the [releases page](https://github.com/ShadelessFox/death-stranding-2-localizer/releases/latest)
2. Copy `winhttp.dll` and `localization.json` files to the game's root directory

### Example

The repository contains a dump of all localizable strings in the game as of version 1.0.49.0.
You don't need to distribute the entire file; instead, include just the changed strings.

Example `localization.json` file:

```json
{
  "06de2c7a-eac7-a045-bcf8-856e4e7b2b6e": {
    "text": "<icon name=gaiji_tutorial_star_full> Begin a new adventure",
    "mode": "default"
  }
}
```

It will change the text "New Game" to "⭐ Begin a new adventure" in the main menu.