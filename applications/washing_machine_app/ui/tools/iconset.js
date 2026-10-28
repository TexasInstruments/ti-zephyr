// Icon set for the MSPM33 washing-machine HMI demo.
// Rasterised by ../../tools/icons.js from ../../tools/lucide-svg — see that file
// for the stroke-scaling rule (Lucide is drawn on a 24px grid with a 2px stroke,
// which renders muddy below ~16px, so stroke widens as the target shrinks).
//
//   node ../tools/icons.js tools/iconset.js images
//
// This panel is 480x320, two thirds the height of the 320x480 siblings, so the
// whole ladder sits one step below theirs: 14px where they use 16, 20 where they
// use 22. Nothing here is decorative — every glyph either labels a program, names
// a piece of TI silicon's I/O, or is the only content of a button.
//
// [lucide name, output name, px, stroke-width]
module.exports = [
  // No wordmark glyph. The branded top bar it belonged to is gone - on a 3.5 inch
  // panel that band was ~18% of the glass spent on a logo.

  // ── The four programs, home screen ──────────────────────────────────────────
  // One glyph per value of subject_program, so the card strip stays exhaustive.
  // These are the largest icons in the app: a program card carries its icon above
  // the name and the icon has to read from a metre away on a booth panel.
  ['shirt',            'icon_prog_cotton', 24, 2.0],
  ['feather',          'icon_prog_eco',    24, 2.0],   // light touch, low temp
  ['droplets',         'icon_prog_synth',  24, 2.0],
  ['timer',            'icon_prog_quick',  24, 2.0],

  // ── Live stats: cycle screen footer, service screen rows ────────────────────
  // 14px, beside 12px meta type. Stroke is heavy because at 14px a 2.0 stroke
  // renders to ~1.2px and greys out next to the text it is meant to anchor.
  ['droplet',          'icon_water',       14, 2.4],   // ADC water level
  ['refresh-cw',       'icon_rpm',         14, 2.4],   // PWM drum speed   // motor current

  // ── Door state. Two glyphs, one lit — the GPIO input made visible. ──────────
  // 13px: they sit inside a header chip whose text is 11px, and a 14px glyph
  // there pushes the pill taller than the 44px bar can hold.

  // ── Controls ────────────────────────────────────────────────────────────────
  // No +/- pair. The steppers they belonged to became a dial and a vertical
  // slider, both dragged rather than nudged.
  // The completion tick, drawn white on the accent disc. Heavier than its size
  // suggests because it is reversed out of a saturated fill, which eats a stroke.

  // ── Cycle phases ────────────────────────────────────────────────────────────
  // One glyph per value of subject_phase, so the phase card stays exhaustive.
  // 20px: they sit above 14px labels in a four-up card and have to carry the row.
  ['droplets',         'icon_ph_pre',      20, 2.2],   // prewash - soaking
  ['shirt',            'icon_ph_wash',     20, 2.2],
  ['droplet',          'icon_ph_rinse',    20, 2.2],   // one drop: clean water
  ['wind',             'icon_ph_spin',     20, 2.2],

  // ── Door state ──────────────────────────────────────────────────────────────
  // Only the OPEN lock exists. A locked door is the normal condition and gets no
  // mark at all - the machine says nothing when nothing is wrong. This glyph and
  // the amber screen border appear together, and both vanish the moment the
  // interlock engages. An icon for "locked" would be a label for silence.
  ['lock-open',        'icon_unlock',      22, 2.3],

  // ── Navigation ──────────────────────────────────────────────────────────────
  ['chevron-left',     'icon_back',        20, 2.2],
  // Two destinations, two different jobs, so two clearly different glyphs. Sliders
  // are the wash settings; a wrench is the service menu. They sit side by side in
  // the top right and a pair that reads as "settings" and "more settings" would be
  // no signpost at all.
  ['sliders-horizontal','icon_options',    20, 2.2],
  ['wrench',           'icon_service',     20, 2.2],

  // The START key is physically below the panel, so the affordance points off the
  // bottom edge of the glass towards it rather than at anything on screen.
  // The two physical push buttons live above and below the panel, so the on-screen
  // hint points off the edge towards the real hardware, not at anything on screen.

  // ── Service screen ──────────────────────────────────────────────────────────
  ['cpu',              'icon_board',       14, 2.4],   // the LP-MSPM33C321A row

  // ── Options screen extras ───────────────────────────────────────────────────
  // 20px, beside a two-line block that stands 39px tall. At 16 the glyph read as
  // a bullet point rather than as the key's mark - it has to hold its own against
  // the title AND the subtitle stacked, not just against one line of type.
  // Deliberately the SAME glyphs
  // the cycle phases use - droplets for a soak, one drop for clean water - because
  // an option that adds a prewash and the prewash phase it adds are the same idea
  // seen twice, and giving them different marks would invent a distinction the
  // machine does not have. Delay start is the exception and takes a clock, which
  // is the only thing on that screen that is about time rather than water.
  ['droplets',         'icon_opt_prewash', 20, 2.2],
  ['droplet',          'icon_opt_rinse',   20, 2.2],
  ['clock',            'icon_opt_delay',   20, 2.2],
];
