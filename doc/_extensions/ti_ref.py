"""Cross-reference role that tolerates pages excluded for the active SoC.

    :ti-ref:`PWM Capture Accuracy <pwm_capture_accuracy>`

If the label belongs to a page that conf.py excluded for this SoC (listed in
``ti_unavailable_labels``), only the title is printed. Every other label is
resolved by the standard ``:ref:`` role, so typos still produce a warning.
"""

from docutils import nodes
from sphinx.util.nodes import split_explicit_title


def ti_ref_role(name, rawtext, text, lineno, inliner, options=None, content=()):
    env = inliner.document.settings.env

    has_title, title, label = split_explicit_title(text)
    if not has_title:
        msg = inliner.reporter.error(
            f"{name} needs an explicit title: :{name}:`Title <label>` (got {text!r})",
            line=lineno,
        )
        return [inliner.problematic(rawtext, rawtext, msg)], [msg]

    if nodes.fully_normalize_name(label) in env.config.ti_unavailable_labels:
        return [nodes.Text(title)], []

    return env.get_domain("std").role("ref")(
        name, rawtext, text, lineno, inliner, options, content
    )


def setup(app):
    app.add_config_value("ti_unavailable_labels", [], "env", types=[list])
    app.add_role("ti-ref", ti_ref_role)
    return {"parallel_read_safe": True, "parallel_write_safe": True}
