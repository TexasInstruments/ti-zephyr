"""Link to a page published next to this site in the workspace docs folder.

    :ws-doc:`DAC Zephyr Documentation <zephyr/hardware/peripherals/dac.html>`

The target is relative to ``workspace_docs_dir`` (<workspace>/docs, see
conf.py), where the Makefile publishes ``zephyr/`` and ``ti-zephyr/<soc>/``.
The ``../`` prefix is computed from the depth of the page the role is used in.
If the target's top-level folder has been published, a missing file is warned
about, so a renamed Zephyr page shows up at build time.
"""

from pathlib import Path

from docutils import nodes
from sphinx.util import logging
from sphinx.util.nodes import split_explicit_title

logger = logging.getLogger(__name__)

# The site is published as <docs>/ti-zephyr/<soc>/<docname>.html
PUBLISH_DEPTH = 2


def ws_doc_role(name, rawtext, text, lineno, inliner, options=None, content=()):
    env = inliner.document.settings.env

    has_title, title, target = split_explicit_title(text)
    if not has_title:
        msg = inliner.reporter.error(
            f"{name} needs an explicit title: :{name}:`Title <path>` (got {text!r})",
            line=lineno,
        )
        return [inliner.problematic(rawtext, rawtext, msg)], [msg]

    docs_dir = Path(env.config.workspace_docs_dir)
    path = target.split("#")[0]
    if (docs_dir / path.split("/")[0]).is_dir() and not (docs_dir / path).is_file():
        logger.warning(
            "%s target not found in %s: %s", name, docs_dir, path,
            location=(env.docname, lineno),
        )

    prefix = "../" * (env.docname.count("/") + PUBLISH_DEPTH)
    return [nodes.reference(rawtext, title, refuri=prefix + target)], []


def setup(app):
    app.add_config_value("workspace_docs_dir", "", "env", types=[str])
    app.add_role("ws-doc", ws_doc_role)
    return {"parallel_read_safe": True, "parallel_write_safe": True}
