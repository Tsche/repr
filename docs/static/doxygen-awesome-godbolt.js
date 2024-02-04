/**
 * Inserts "Run in compiler explorer" buttons in code blocks
 * iff their code's first line starts with //*
 */

class DoxygenAwesomeGodbolt extends HTMLElement {
  constructor() {
    super();
    this.onclick  = this.runContent;
    this.compiler = {};
  }
  static title = 'Run in Compiler Explorer';
  static runIcon =
      `<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="24" height="24"><path d="M 12 0 C 5.373 0 0 5.373 0 12 s 5.373 12 12 12 s 12 -5.373 12 -12 S 18.627 0 12 0 z M 7.5 18 V 6 l 12.006 6 L 7.5 18 z" style="fill:#32a852;"/></svg>`;
  static successIcon =
      `<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="24" height="24"><path d="M0 0h24v24H0V0z" fill="none"/><path d="M9 16.17L4.83 12l-1.42 1.41L9 19 21 7l-1.41-1.41L9 16.17z"/></svg>`;
  static successDuration        = 980;
  static defaultCompiler        = 'gsnapshot';
  static defaultCompilerOptions = '-std=c++20 -O3';

  static init() {
    $(() => {
      const fragments = document.getElementsByClassName('fragment');
      for (const fragment of fragments) {
        let magic_line = DoxygenAwesomeGodbolt.getContent(fragment).trimStart();
        if (magic_line.startsWith('//*')) {
          // extract only first line
          magic_line = magic_line.split('\n', 1)[0];
        } else {
          // special line not found => not runnable
          continue;
        }

        for (const child of fragment.children) {
          // trim whitespace and magic line
          const text = child.innerHTML;
          const node = child.firstChild;
          if (text.trim() != '' && !(node.className == 'comment' && node.innerHTML.startsWith('//*'))) {
            break;
          }
          child.remove();
        }

        const fragmentWrapper     = document.createElement('div');
        fragmentWrapper.className = 'doxygen-awesome-fragment-wrapper';
        const godboltButton       = document.createElement('doxygen-awesome-godbolt');
        godboltButton.innerHTML   = DoxygenAwesomeGodbolt.runIcon;
        godboltButton.title       = DoxygenAwesomeGodbolt.title;
        godboltButton.compiler    = DoxygenAwesomeGodbolt.parseCompilerSettings(magic_line);

        fragment.parentNode.replaceChild(fragmentWrapper, fragment);
        fragmentWrapper.appendChild(fragment);
        fragmentWrapper.appendChild(godboltButton);
      }
    })
  }

  runContent() {
    const content   = this.previousSibling.cloneNode(true);
    let textContent = DoxygenAwesomeGodbolt.getContent(content);

    // Fix include
    textContent       = textContent.replace('#include <repr>', '#include <https://repr.palliate.io/amalgamated.h>')
    const clientstate = {
      'sessions': [{
        'id': 1,
        'language': 'c++',
        'source': textContent.trim() + '\n',
        'compilers': [this.compiler],
        'executors': [{'compiler': this.compiler}]
      }]
    };
    const encoded = btoa(unescape(encodeURIComponent(JSON.stringify(clientstate))));
    window.open(`https://godbolt.org/clientstate/${encoded}`, '_blank').focus();

    this.innerHTML = DoxygenAwesomeGodbolt.successIcon;
    window.setTimeout(() => {
      this.classList.remove('success');
      this.innerHTML = DoxygenAwesomeGodbolt.runIcon;
    }, DoxygenAwesomeGodbolt.successDuration);
  }

  static getContent(node) {
    node.querySelectorAll('.lineno, .ttc').forEach((node) => {node.remove()});
    let {textContent} = node;
    return textContent;
  }

  static parseCompilerSettings(magic_line) {
    if (!magic_line.startsWith('//*')) {
      return {};
    }

    magic_line = magic_line.substr(3).trim();
    if (!magic_line) {
      return {'id': DoxygenAwesomeGodbolt.defaultCompiler, 'options': DoxygenAwesomeGodbolt.defaultCompilerOptions};
    } else if (magic_line.startsWith('-')) {
      // no compiler set, but got options
      return {'id': DoxygenAwesomeGodbolt.defaultCompiler, 'options': magic_line};
    } else if (!magic_line.includes('-')) {
      // no options, but got compiler
      return {'id': magic_line, 'options': DoxygenAwesomeGodbolt.defaultCompilerOptions};
    } else {
      const [head]              = magic_line.split('-', 1);
      const [compiler, options] = [head, magic_line.substr(head.length + 1)];
      return {'id': compiler.trim(), 'options': '-' + options};
    }
  }
}

customElements.define('doxygen-awesome-godbolt', DoxygenAwesomeGodbolt);
