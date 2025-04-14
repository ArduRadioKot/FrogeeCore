// Инициализация приложения
document.addEventListener('DOMContentLoaded', () => {
    let tabs = [];
    let activeTabId = null;
    let tabCounter = 0;

    // Определяем цветовые схемы для тем
    const themes = {
        // Темные темы
        'dracula': {
            '--bg-color': '#282a36',
            '--text-color': '#f8f8f2',
            '--toolbar-bg': '#44475a',
            '--button-bg': '#6272a4',
            '--button-hover': '#bd93f9',
            '--border-color': '#6272a4',
            '--tab-active-bg': '#44475a',
            '--tab-inactive-bg': '#282a36'
        },
        'monokai': {
            '--bg-color': '#272822',
            '--text-color': '#f8f8f2',
            '--toolbar-bg': '#3e3d32',
            '--button-bg': '#75715e',
            '--button-hover': '#f92672',
            '--border-color': '#75715e',
            '--tab-active-bg': '#3e3d32',
            '--tab-inactive-bg': '#272822'
        },
        'material': {
            '--bg-color': '#263238',
            '--text-color': '#EEFFFF',
            '--toolbar-bg': '#37474F',
            '--button-bg': '#546E7A',
            '--button-hover': '#82AAFF',
            '--border-color': '#546E7A',
            '--tab-active-bg': '#37474F',
            '--tab-inactive-bg': '#263238'
        },
        'nord': {
            '--bg-color': '#2E3440',
            '--text-color': '#D8DEE9',
            '--toolbar-bg': '#3B4252',
            '--button-bg': '#434C5E',
            '--button-hover': '#88C0D0',
            '--border-color': '#434C5E',
            '--tab-active-bg': '#3B4252',
            '--tab-inactive-bg': '#2E3440'
        },
        // Светлые темы
        'github-light': {
            '--bg-color': '#ffffff',
            '--text-color': '#24292e',
            '--toolbar-bg': '#f6f8fa',
            '--button-bg': '#e1e4e8',
            '--button-hover': '#0366d6',
            '--border-color': '#e1e4e8',
            '--tab-active-bg': '#ffffff',
            '--tab-inactive-bg': '#f6f8fa'
        },
        'solarized-light': {
            '--bg-color': '#fdf6e3',
            '--text-color': '#657b83',
            '--toolbar-bg': '#eee8d5',
            '--button-bg': '#93a1a1',
            '--button-hover': '#2aa198',
            '--border-color': '#93a1a1',
            '--tab-active-bg': '#fdf6e3',
            '--tab-inactive-bg': '#eee8d5'
        },
        'eclipse': {
            '--bg-color': '#ffffff',
            '--text-color': '#000000',
            '--toolbar-bg': '#f5f5f5',
            '--button-bg': '#e4e4e4',
            '--button-hover': '#4b9edd',
            '--border-color': '#c8c8c8',
            '--tab-active-bg': '#ffffff',
            '--tab-inactive-bg': '#f5f5f5'
        },
        'xcode-light': {
            '--bg-color': '#ffffff',
            '--text-color': '#1f1f24',
            '--toolbar-bg': '#f2f2f7',
            '--button-bg': '#e5e5ea',
            '--button-hover': '#007aff',
            '--border-color': '#d1d1d6',
            '--tab-active-bg': '#ffffff',
            '--tab-inactive-bg': '#f2f2f7'
        }
    };

    // Функция применения темы
    function applyTheme(themeName) {
        const theme = themes[themeName];
        if (theme) {
            Object.entries(theme).forEach(([key, value]) => {
                document.documentElement.style.setProperty(key, value);
            });
        }
        
        // Обновляем тему для всех редакторов
        tabs.forEach(tab => {
            const themeMappings = {
                'github-light': 'default',
                'solarized-light': 'solarized',
                'eclipse': 'eclipse',
                'xcode-light': 'xq-light',
                'dracula': 'dracula',
                'monokai': 'monokai',
                'material': 'material',
                'nord': 'nord'
            };
            tab.editor.setOption('theme', themeMappings[themeName] || 'dracula');
        });
    }

    // Функция создания нового редактора
    function createEditor() {
        const wrapper = document.createElement('div');
        wrapper.className = 'editor-wrapper';
        const textarea = document.createElement('textarea');
        document.getElementById('editors-container').appendChild(wrapper);
        wrapper.appendChild(textarea);

        const currentTheme = document.getElementById('theme').value;
        const themeMappings = {
            'github-light': 'default',
            'solarized-light': 'solarized',
            'eclipse': 'eclipse',
            'xcode-light': 'xq-light',
            'dracula': 'dracula',
            'monokai': 'monokai',
            'material': 'material',
            'nord': 'nord'
        };
        const codeMirrorTheme = themeMappings[currentTheme] || 'dracula';

        const editor = CodeMirror.fromTextArea(textarea, {
            lineNumbers: true,
            mode: "javascript",
            theme: codeMirrorTheme,
            autoCloseBrackets: true,
            matchBrackets: true,
            indentUnit: 4,
            tabSize: 4,
            lineWrapping: true,
            styleActiveLine: true,
            viewportMargin: Infinity,
            scrollbarStyle: "simple",
            readOnly: false,
            inputStyle: "textarea",
            autofocus: true,
            dragDrop: true,
            cursorHeight: 1,
            showCursorWhenSelecting: true,
            spellcheck: true,
            autocorrect: true,
            autocapitalize: true
        });

        editor.setSize("100%", "100%");
        editor.refresh();
        return { editor, wrapper };
    }

    // Функция создания новой вкладки
    function createTab(filename = 'Untitled') {
        const tabId = `tab-${tabCounter++}`;
        const { editor, wrapper } = createEditor();
        
        const tab = document.createElement('div');
        tab.className = 'tab';
        tab.setAttribute('data-tab-id', tabId);
        
        const tabTitle = document.createElement('div');
        tabTitle.className = 'tab-title';
        tabTitle.textContent = filename;
        
        const closeButton = document.createElement('button');
        closeButton.className = 'tab-close';
        closeButton.innerHTML = '×';
        closeButton.onclick = (e) => {
            e.stopPropagation();
            closeTab(tabId);
        };
        
        tab.appendChild(tabTitle);
        tab.appendChild(closeButton);
        
        tab.onclick = () => activateTab(tabId);
        
        document.getElementById('tabs-container').insertBefore(
            tab,
            document.getElementById('new-tab-button')
        );
        
        const tabData = {
            id: tabId,
            filename: filename,
            editor: editor,
            wrapper: wrapper,
            tab: tab,
            path: null
        };
        
        tabs.push(tabData);
        activateTab(tabId);
        return tabData;
    }

    // Функция активации вкладки
    function activateTab(tabId) {
        tabs.forEach(tab => {
            if (tab.id === tabId) {
                tab.tab.classList.add('active');
                tab.wrapper.classList.add('active');
                tab.editor.refresh();
                tab.editor.focus();
            } else {
                tab.tab.classList.remove('active');
                tab.wrapper.classList.remove('active');
            }
        });
        activeTabId = tabId;
    }

    // Функция закрытия вкладки
    function closeTab(tabId) {
        const index = tabs.findIndex(tab => tab.id === tabId);
        if (index === -1) return;

        const tab = tabs[index];
        tab.tab.remove();
        tab.wrapper.remove();
        tabs.splice(index, 1);

        if (tabs.length === 0) {
            createTab();
        } else if (activeTabId === tabId) {
            activateTab(tabs[Math.min(index, tabs.length - 1)].id);
        }
    }

    // Функция получения активной вкладки
    function getActiveTab() {
        return tabs.find(tab => tab.id === activeTabId);
    }

    // Создаем первую вкладку
    createTab();

    // Обработчик кнопки новой вкладки
    document.getElementById('new-tab-button').addEventListener('click', () => {
        createTab();
    });

    // Обработчик изменения темы
    document.getElementById('theme').addEventListener('change', (e) => {
        applyTheme(e.target.value);
    });

    // Обработчик открытия файла
    document.getElementById('open').addEventListener('click', () => {
        const input = document.createElement('input');
        input.type = 'file';
        input.accept = '.js,.html,.css,.json,.txt';
        input.onchange = (e) => {
            const file = e.target.files[0];
            if (file) {
                const reader = new FileReader();
                reader.onload = (e) => {
                    const tab = createTab(file.name);
                    tab.editor.setValue(e.target.result);
                };
                reader.readAsText(file);
            }
        };
        input.click();
    });

    // Обработчик сохранения файла
    document.getElementById('save').addEventListener('click', () => {
        const activeTab = getActiveTab();
        if (!activeTab) return;

        const content = activeTab.editor.getValue();
        const blob = new Blob([content], { type: 'text/plain' });
        const url = URL.createObjectURL(blob);
        const a = document.createElement('a');
        a.href = url;
        a.download = activeTab.filename;
        document.body.appendChild(a);
        a.click();
        document.body.removeChild(a);
        URL.revokeObjectURL(url);
    });

    // Применяем начальную тему
    applyTheme(document.getElementById('theme').value);
});