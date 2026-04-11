# Localization mod for Death Stranding 2

## Installation

1. Download the latest release from the [releases page](https://github.com/ShadelessFox/death-stranding-2-localizer/releases/latest)
2. Copy `winhttp.dll` and `localization.json` files to the game's root directory

### Example

The repository contains a dump of all localizable strings in the game as of version 1.1.53.0.
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

The `mode` specifies when the text is shown:

- `default` the text will be shown only if subtitles are enabled
- `always` the text will be shown always regardless of subtitle options
- `never` the text will never be shown regradless of subtitle options

> [!NOTE]
> You may want to enforce subtitles to be shown for in-game text written in a foreign language. And vice versa, hide
> subtitles for in-game text written in the same language.

The example above changes the text "New Game" to "⭐ Begin a new adventure" in the main menu.

> [!IMPORTANT]
> Some texts consist purely of `　` (U+3000) or other whitespace symbols. Trimming them **will** result
> in the game showing placeholder text `« No translated or source text`.
> 
> To figure out which object corresponds to a particular in-game text, you can replace `localization.json` with `localization_debug.json`
> (don't forget to rename it). It contains all texts with a UUID prefix corresponding to the entry in the file.

## References

This software wouldn't have been possible without the contributions of other people.
It was directly inspired by, and contains code borrowed from the following software:
- https://github.com/Nukem9/HZDCoreEditor, by [Nukem9](https://github.com/Nukem9)
- https://github.com/Nukem9/hfw-gameplay-tweaks, by [Nukem9](https://github.com/Nukem9)
- https://github.com/Nukem9/QuickDllProxy, licensed under LGPL-3.0, by [Nukem9](https://github.com/Nukem9)

## License

This software is licensed under the GPL-3.0 license.

This software is not sponsored by or related to Guerrilla Games, Kojima Productions, Sony Interactive Entertainment, or others.

Source code and all software made with Decima engine belong to their developers.
