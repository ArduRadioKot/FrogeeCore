// Глобальные переменные
let editors = [];
let activeEditorIndex = -1;

// Функция обновления темы интерфейса
function updateThemeColors(theme) {
    const root = document.documentElement;
    
    // Определяем цвета для каждой темы
    const themeColors = {
        'dracula': {
            bg: '#282a36',
            text: '#f8f8f2',
            toolbar: '#44475a',
            button: '#6272a4',
            buttonHover: '#bd93f9',
            border: '#6272a4',
            tabActive: '#44475a',
            tabInactive: '#282a36'
        },
        'monokai': {
            bg: '#272822',
            text: '#f8f8f2',
            toolbar: '#3e3d32',
            button: '#75715e',
            buttonHover: '#a6e22e',
            border: '#49483e',
            tabActive: '#3e3d32',
            tabInactive: '#272822'
        },
        'material': {
            bg: '#263238',
            text: '#EEFFFF',
            toolbar: '#37474f',
            button: '#546e7a',
            buttonHover: '#82AAFF',
            border: '#405c6b',
            tabActive: '#37474f',
            tabInactive: '#263238'
        },
        'nord': {
            bg: '#2e3440',
            text: '#eceff4',
            toolbar: '#3b4252',
            button: '#4c566a',
            buttonHover: '#88c0d0',
            border: '#434c5e',
            tabActive: '#3b4252',
            tabInactive: '#2e3440'
        },
        'eclipse': {
            bg: '#ffffff',
            text: '#000000',
            toolbar: '#f0f0f0',
            button: '#e0e0e0',
            buttonHover: '#cccccc',
            border: '#c0c0c0',
            tabActive: '#ffffff',
            tabInactive: '#f0f0f0'
        },
        'solarized': {
            bg: '#fdf6e3',
            text: '#657b83',
            toolbar: '#eee8d5',
            button: '#93a1a1',
            buttonHover: '#586e75',
            border: '#eee8d5',
            tabActive: '#fdf6e3',
            tabInactive: '#eee8d5'
        },
        'idea': {
            bg: '#ffffff',
            text: '#000000',
            toolbar: '#f5f5f5',
            button: '#e2e2e2',
            buttonHover: '#c8c8c8',
            border: '#d4d4d4',
            tabActive: '#ffffff',
            tabInactive: '#f5f5f5'
        },
        'xq-light': {
            bg: '#ffffff',
            text: '#000000',
            toolbar: '#f7f7f7',
            button: '#e8e8e8',
            buttonHover: '#d1d1d1',
            border: '#e8e8e8',
            tabActive: '#ffffff',
            tabInactive: '#f7f7f7'
        }
    };

    // Получаем цвета для текущей темы или используем светлую тему по умолчанию
    const colors = themeColors[theme] || themeColors['eclipse'];

    // Применяем цвета
    root.style.setProperty('--bg-color', colors.bg);
    root.style.setProperty('--text-color', colors.text);
    root.style.setProperty('--toolbar-bg', colors.toolbar);
    root.style.setProperty('--button-bg', colors.button);
    root.style.setProperty('--button-hover', colors.buttonHover);
    root.style.setProperty('--border-color', colors.border);
    root.style.setProperty('--tab-active-bg', colors.tabActive);
    root.style.setProperty('--tab-inactive-bg', colors.tabInactive);

    // Обновляем стили для CodeMirror
    document.body.style.backgroundColor = colors.bg;
    const editorWrappers = document.querySelectorAll('.editor-wrapper, .minimap-wrapper');
    editorWrappers.forEach(wrapper => {
        wrapper.style.backgroundColor = colors.bg;
    });
}

// Функция для создания нового редактора
function createEditor() {
    const editorWrapper = document.createElement('div');
    editorWrapper.className = 'editor-wrapper';
    document.getElementById('editors-container').appendChild(editorWrapper);

    const currentTheme = document.getElementById('theme').value || 'dracula';
    updateThemeColors(currentTheme);

    const editor = CodeMirror(editorWrapper, {
        mode: 'javascript',
        theme: currentTheme,
        lineNumbers: true,
        autoCloseBrackets: true,
        matchBrackets: true,
        styleActiveLine: true,
        scrollbarStyle: 'overlay',
        lineWrapping: true,
        tabSize: 4,
        indentUnit: 4,
        indentWithTabs: true
    });

    // Создаем миниатюру
    const minimapWrapper = document.createElement('div');
    minimapWrapper.className = 'minimap-wrapper';
    document.getElementById('editors-container').appendChild(minimapWrapper);

    const minimap = CodeMirror(minimapWrapper, {
        value: editor.getValue(),
        mode: editor.getOption('mode'),
        theme: editor.getOption('theme'),
        readOnly: true,
        lineWrapping: true,
        scrollbarStyle: null
    });

    // Создаем слайдер для миниатюры
    const slider = document.createElement('div');
    slider.className = 'minimap-slider';
    minimapWrapper.appendChild(slider);

    // Устанавливаем масштаб миниатюры
    const minimapElement = minimapWrapper.querySelector('.CodeMirror');
    minimapElement.classList.add('minimap');
    minimapElement.style.transform = 'scale(0.17)';
    minimapElement.style.width = '588%';

    // Обновляем миниатюру при изменении содержимого
    editor.on('change', () => {
        minimap.setValue(editor.getValue());
        updateMinimapScroll();
    });

    // Обновляем положение слайдера при прокрутке
    editor.on('scroll', updateMinimapScroll);

    function updateMinimapScroll() {
        const editorInfo = editor.getScrollInfo();
        const minimapInfo = minimap.getScrollInfo();
        
        // Обновляем положение и размер слайдера
        const sliderHeight = Math.max(30, (editorInfo.clientHeight / editorInfo.height) * minimapInfo.clientHeight);
        const sliderTop = (editorInfo.top / editorInfo.height) * minimapInfo.clientHeight;
        
        slider.style.height = `${sliderHeight}px`;
        slider.style.top = `${sliderTop}px`;
        
        // Синхронизируем прокрутку миниатюры
        const scrollRatio = editorInfo.top / (editorInfo.height - editorInfo.clientHeight);
        const minimapScrollTop = scrollRatio * (minimapInfo.height - minimapInfo.clientHeight);
        minimap.scrollTo(null, minimapScrollTop);
    }

    // Обработка клика по миниатюре для прокрутки
    minimapWrapper.addEventListener('click', (e) => {
        if (e.target.classList.contains('minimap-slider')) return;
        
        const minimapInfo = minimap.getScrollInfo();
        const editorInfo = editor.getScrollInfo();
        
        const clickRatio = (e.offsetY - slider.offsetHeight / 2) / minimapInfo.clientHeight;
        const scrollTop = clickRatio * (editorInfo.height - editorInfo.clientHeight);
        
        editor.scrollTo(null, Math.max(0, Math.min(scrollTop, editorInfo.height - editorInfo.clientHeight)));
    });

    // Добавляем перетаскивание слайдера
    let isDragging = false;
    let startY = 0;
    let startTop = 0;

    slider.addEventListener('mousedown', (e) => {
        isDragging = true;
        startY = e.clientY;
        startTop = parseFloat(slider.style.top) || 0;
        document.body.style.userSelect = 'none';
        slider.classList.add('dragging');
    });

    document.addEventListener('mousemove', (e) => {
        if (!isDragging) return;
        
        const delta = e.clientY - startY;
        const minimapInfo = minimap.getScrollInfo();
        const editorInfo = editor.getScrollInfo();
        
        const newTop = Math.max(0, Math.min(startTop + delta, minimapInfo.clientHeight - slider.offsetHeight));
        const scrollRatio = newTop / minimapInfo.clientHeight;
        const editorScrollTop = scrollRatio * (editorInfo.height - editorInfo.clientHeight);
        
        editor.scrollTo(null, editorScrollTop);
    });

    document.addEventListener('mouseup', () => {
        if (isDragging) {
            isDragging = false;
            document.body.style.userSelect = '';
            slider.classList.remove('dragging');
        }
    });

    // Добавляем обработку колесика мыши над миниатюрой
    minimapWrapper.addEventListener('wheel', (e) => {
        e.preventDefault();
        const editorInfo = editor.getScrollInfo();
        const scrollAmount = e.deltaY * 0.5;
        const newScrollTop = Math.max(0, Math.min(
            editorInfo.top + scrollAmount,
            editorInfo.height - editorInfo.clientHeight
        ));
        editor.scrollTo(null, newScrollTop);
    });

    return { editor, wrapper: editorWrapper, minimap, minimapWrapper };
}

// Функция для создания новой вкладки
function createTab(index) {
    const tab = document.createElement('div');
    tab.className = 'tab';
    tab.setAttribute('data-index', index);

    const title = document.createElement('div');
    title.className = 'tab-title';
    title.textContent = `Untitled-${index + 1}`;

    const closeButton = document.createElement('button');
    closeButton.className = 'tab-close';
    closeButton.innerHTML = '×';
    closeButton.onclick = (e) => {
        e.stopPropagation();
        closeTab(index);
    };

    tab.appendChild(title);
    tab.appendChild(closeButton);

    tab.onclick = () => activateTab(index);

    const newTabButton = document.getElementById('new-tab-button');
    document.getElementById('tabs-container').insertBefore(tab, newTabButton);

    return tab;
}

// Функция для активации вкладки
function activateTab(index) {
    // Деактивировать текущую активную вкладку
    const tabs = document.querySelectorAll('.tab');
    tabs.forEach(tab => tab.classList.remove('active'));
    
    const editorWrappers = document.querySelectorAll('.editor-wrapper');
    editorWrappers.forEach(wrapper => wrapper.classList.remove('active'));
    
    const minimapWrappers = document.querySelectorAll('.minimap-wrapper');
    minimapWrappers.forEach(wrapper => wrapper.classList.remove('active'));

    // Активировать новую вкладку
    if (editors[index]) {
        tabs[index].classList.add('active');
        editors[index].wrapper.classList.add('active');
        editors[index].minimapWrapper.classList.add('active');
        editors[index].editor.refresh();
        editors[index].minimap.refresh();
        activeEditorIndex = index;
    }
}

// Функция для закрытия вкладки
function closeTab(index) {
    if (editors.length <= 1) return; // Не закрывать последнюю вкладку

    // Удалить вкладку и редактор
    const tabs = document.querySelectorAll('.tab');
    tabs[index].remove();
    editors[index].wrapper.remove();
    editors[index].minimapWrapper.remove();
    editors.splice(index, 1);

    // Обновить индексы оставшихся вкладок
    document.querySelectorAll('.tab').forEach((tab, i) => {
        tab.setAttribute('data-index', i);
        tab.querySelector('.tab-title').textContent = `Untitled-${i + 1}`;
    });

    // Активировать ближайшую вкладку
    if (activeEditorIndex === index) {
        const newIndex = Math.min(index, editors.length - 1);
        activateTab(newIndex);
    }
}

// Обработчик изменения темы
document.getElementById('theme').addEventListener('change', (e) => {
    const theme = e.target.value;
    editors.forEach(({ editor, minimap }) => {
        editor.setOption('theme', theme);
        minimap.setOption('theme', theme);
    });
    updateThemeColors(theme);
});

// Создание первой вкладки при загрузке
document.addEventListener('DOMContentLoaded', () => {
    createNewTab();
});

// Обработчик кнопки новой вкладки
document.getElementById('new-tab-button').addEventListener('click', createNewTab);

// Функция создания новой вкладки
function createNewTab() {
    const index = editors.length;
    const { editor, wrapper, minimap, minimapWrapper } = createEditor();
    editors.push({ editor, wrapper, minimap, minimapWrapper });
    createTab(index);
    activateTab(index);
}

// Обработчики кнопок
document.getElementById('open').addEventListener('click', () => {
    // Создаем input для выбора файла
    const input = document.createElement('input');
    input.type = 'file';
    input.accept = '.txt,.js,.py,.html,.css,.json,.md,.cpp,.c,.java,.swift';

    input.onchange = async (e) => {
        const file = e.target.files[0];
        if (!file) return;

        try {
            // Читаем содержимое файла
            const content = await new Promise((resolve, reject) => {
                const reader = new FileReader();
                reader.onload = (e) => resolve(e.target.result);
                reader.onerror = (e) => reject(e);
                reader.readAsText(file);
            });

            // Создаем новую вкладку
            const index = editors.length;
            const { editor, wrapper, minimap, minimapWrapper } = createEditor();
            editors.push({ editor, wrapper, minimap, minimapWrapper, fileName: file.name });
            
            // Устанавливаем содержимое и режим редактора на основе расширения файла
            editor.setValue(content);
            const extension = file.name.split('.').pop().toLowerCase();
            const modeMap = {
                'js': 'javascript',
                'py': 'python',
                'html': 'xml',
                'css': 'css',
                'cpp': 'text/x-c++src',
                'c': 'text/x-csrc',
                'java': 'text/x-java',
                'swift': 'swift'
            };
            if (modeMap[extension]) {
                editor.setOption('mode', modeMap[extension]);
            }

            // Создаем вкладку с именем файла
            const tab = createTab(index);
            const titleElement = tab.querySelector('.tab-title');
            titleElement.textContent = file.name;
            
            activateTab(index);
        } catch (err) {
            console.error('Ошибка при открытии файла:', err);
            alert('Не удалось открыть файл: ' + err.message);
        }
    };

    input.click();
});

document.getElementById('save').addEventListener('click', () => {
    if (activeEditorIndex === -1) return;
    
    const currentEditor = editors[activeEditorIndex];
    try {
        // Создаем Blob из содержимого редактора
        const content = currentEditor.editor.getValue();
        const blob = new Blob([content], { type: 'text/plain' });
        
        // Создаем ссылку для скачивания
        const a = document.createElement('a');
        a.href = URL.createObjectURL(blob);
        
        // Используем существующее имя файла или создаем новое
        const fileName = currentEditor.fileName || 'untitled.txt';
        a.download = fileName;
        
        // Добавляем ссылку в документ, кликаем по ней и удаляем
        document.body.appendChild(a);
        a.click();
        document.body.removeChild(a);
        
        // Освобождаем URL
        URL.revokeObjectURL(a.href);
        
        // Сохраняем имя файла в редакторе
        if (!currentEditor.fileName) {
            currentEditor.fileName = fileName;
            const tab = document.querySelector(`.tab[data-index="${activeEditorIndex}"]`);
            const titleElement = tab.querySelector('.tab-title');
            titleElement.textContent = fileName;
        }
    } catch (err) {
        console.error('Ошибка при сохранении файла:', err);
        alert('Не удалось сохранить файл: ' + err.message);
    }
});

document.getElementById('quit').addEventListener('click', () => {
    window.close();
});

// Функция для создания элемента дерева файлов
function createTreeItem(name, isFolder, path) {
    const item = document.createElement('div');
    item.className = 'tree-item';
    item.dataset.path = path;

    if (isFolder) {
        const toggle = document.createElement('div');
        toggle.className = 'tree-item-toggle';
        toggle.textContent = '›';
        item.appendChild(toggle);

        const icon = document.createElement('div');
        icon.className = 'tree-item-icon';
        icon.textContent = '▸';
        item.appendChild(icon);

        const children = document.createElement('div');
        children.className = 'tree-children';
        children.dataset.collapsed = 'false';

        toggle.onclick = (e) => {
            e.stopPropagation();
            const isCollapsed = children.dataset.collapsed === 'true';
            children.dataset.collapsed = !isCollapsed;
            toggle.style.transform = isCollapsed ? 'rotate(90deg)' : '';
            icon.textContent = isCollapsed ? '▾' : '▸';
        };

        item.after(children);
    } else {
        const spacer = document.createElement('div');
        spacer.style.width = '16px';
        item.appendChild(spacer);

        const icon = document.createElement('div');
        icon.className = 'tree-item-icon';
        
        // Определяем иконку на основе расширения файла
        const extension = name.split('.').pop().toLowerCase();
        const iconMap = {
            'js': '{}',
            'py': '⟡',
            'html': '⟨⟩',
            'css': '⚡',
            'json': '⦿',
            'md': '▤',
            'txt': '▤',
            'cpp': '⧉',
            'c': '⧉',
            'h': '⧉',
            'java': '☕',
            'swift': '⚡',
            'default': '▤'
        };
        icon.textContent = iconMap[extension] || iconMap.default;
        item.appendChild(icon);

        item.onclick = async () => {
            try {
                const fileHandle = await item.fileHandle;
                const file = await fileHandle.getFile();
                const content = await file.text();

                // Создаем новую вкладку
                const index = editors.length;
                const { editor, wrapper, minimap, minimapWrapper } = createEditor();
                editors.push({ editor, wrapper, minimap, minimapWrapper, fileHandle });

                // Устанавливаем содержимое и режим редактора
                editor.setValue(content);
                const extension = file.name.split('.').pop().toLowerCase();
                const modeMap = {
                    'js': 'javascript',
                    'py': 'python',
                    'html': 'xml',
                    'css': 'css',
                    'cpp': 'text/x-c++src',
                    'c': 'text/x-csrc',
                    'java': 'text/x-java',
                    'swift': 'swift'
                };
                if (modeMap[extension]) {
                    editor.setOption('mode', modeMap[extension]);
                }

                // Создаем вкладку
                const tab = createTab(index);
                const titleElement = tab.querySelector('.tab-title');
                titleElement.textContent = file.name;

                activateTab(index);

                // Подсвечиваем текущий файл в дереве
                document.querySelectorAll('.tree-item.active').forEach(el => el.classList.remove('active'));
                item.classList.add('active');
            } catch (err) {
                console.error('Ошибка при открытии файла:', err);
                alert('Не удалось открыть файл: ' + err.message);
            }
        };
    }

    const label = document.createElement('div');
    label.className = 'tree-item-label';
    label.textContent = name;
    item.appendChild(label);

    return item;
}

// Функция для построения дерева файлов
async function buildFileTree(handle, container) {
    if (handle.kind === 'file') {
        const item = createTreeItem(handle.name, false, handle.name);
        item.fileHandle = Promise.resolve(handle);
        container.appendChild(item);
    } else if (handle.kind === 'directory') {
        const item = createTreeItem(handle.name, true, handle.name);
        container.appendChild(item);

        const children = document.createElement('div');
        children.className = 'tree-children';
        container.appendChild(children);

        try {
            for await (const entry of handle.values()) {
                await buildFileTree(entry, children);
            }
        } catch (err) {
            console.error('Ошибка при чтении директории:', err);
        }
    }
}

// Обработчик кнопки открытия папки
document.getElementById('open-folder').addEventListener('click', () => {
    const input = document.createElement('input');
    input.type = 'file';
    input.webkitdirectory = true;
    input.directory = true;
    input.multiple = true;

    input.onchange = async (e) => {
        const files = Array.from(e.target.files);
        if (!files.length) return;

        const fileExplorer = document.getElementById('file-explorer');
        const fileTree = document.getElementById('file-tree');
        
        // Очищаем текущее дерево
        fileTree.innerHTML = '';

        // Создаем структуру дерева
        const tree = {};
        files.forEach(file => {
            const parts = file.webkitRelativePath.split('/');
            let current = tree;
            
            // Создаем структуру папок
            for (let i = 0; i < parts.length - 1; i++) {
                if (!current[parts[i]]) {
                    current[parts[i]] = {
                        type: 'folder',
                        children: {}
                    };
                }
                current = current[parts[i]].children;
            }
            
            // Добавляем файл
            current[parts[parts.length - 1]] = {
                type: 'file',
                file: file
            };
        });

        // Функция для создания элементов дерева
        function createTreeElements(structure, parentPath = '') {
            const container = document.createElement('div');
            container.className = 'tree-children';
            
            Object.entries(structure).forEach(([name, data]) => {
                const path = parentPath ? `${parentPath}/${name}` : name;
                const item = createTreeItem(name, data.type === 'folder', path);
                container.appendChild(item);

                if (data.type === 'file') {
                    item.onclick = async () => {
                        try {
                            const content = await new Promise((resolve, reject) => {
                                const reader = new FileReader();
                                reader.onload = (e) => resolve(e.target.result);
                                reader.onerror = reject;
                                reader.readAsText(data.file);
                            });

                            // Создаем новую вкладку
                            const index = editors.length;
                            const { editor, wrapper, minimap, minimapWrapper } = createEditor();
                            editors.push({ editor, wrapper, minimap, minimapWrapper, fileName: name });

                            // Устанавливаем содержимое и режим редактора
                            editor.setValue(content);
                            const extension = name.split('.').pop().toLowerCase();
                            const modeMap = {
                                'js': 'javascript',
                                'py': 'python',
                                'html': 'xml',
                                'css': 'css',
                                'cpp': 'text/x-c++src',
                                'c': 'text/x-csrc',
                                'java': 'text/x-java',
                                'swift': 'swift'
                            };
                            if (modeMap[extension]) {
                                editor.setOption('mode', modeMap[extension]);
                            }

                            // Создаем вкладку
                            const tab = createTab(index);
                            const titleElement = tab.querySelector('.tab-title');
                            titleElement.textContent = name;

                            activateTab(index);

                            // Подсвечиваем текущий файл в дереве
                            document.querySelectorAll('.tree-item.active').forEach(el => el.classList.remove('active'));
                            item.classList.add('active');
                        } catch (err) {
                            console.error('Ошибка при открытии файла:', err);
                            alert('Не удалось открыть файл: ' + err.message);
                        }
                    };
                } else if (data.type === 'folder') {
                    const childrenContainer = createTreeElements(data.children, path);
                    childrenContainer.dataset.collapsed = 'false';
                    container.appendChild(childrenContainer);

                    const toggle = item.querySelector('.tree-item-toggle');
                    const icon = item.querySelector('.tree-item-icon');
                    
                    toggle.onclick = (e) => {
                        e.stopPropagation();
                        const isCollapsed = childrenContainer.dataset.collapsed === 'true';
                        childrenContainer.dataset.collapsed = !isCollapsed;
                        toggle.style.transform = isCollapsed ? 'rotate(90deg)' : '';
                        icon.textContent = isCollapsed ? '▾' : '▸';
                    };
                }
            });

            return container;
        }

        // Строим дерево
        const rootName = files[0].webkitRelativePath.split('/')[0];
        const rootItem = createTreeItem(rootName, true, rootName);
        fileTree.appendChild(rootItem);

        const rootChildren = createTreeElements(tree[rootName].children, rootName);
        fileTree.appendChild(rootChildren);

        // Показываем панель проводника
        fileExplorer.classList.add('active');
    };

    input.click();
});

// Обновляем обработчик кнопки сворачивания всех папок
document.getElementById('collapse-all').addEventListener('click', () => {
    const fileTree = document.getElementById('file-tree');
    const allChildren = fileTree.querySelectorAll('.tree-children');
    const allToggles = fileTree.querySelectorAll('.tree-item-toggle');
    const allFolderIcons = fileTree.querySelectorAll('.tree-item-icon');

    allChildren.forEach(child => {
        child.dataset.collapsed = 'true';
    });

    allToggles.forEach(toggle => {
        toggle.style.transform = '';
    });

    allFolderIcons.forEach(icon => {
        if (icon.textContent === '▾' || icon.textContent === '▸') {
            icon.textContent = '▸';
        }
    });
}); 