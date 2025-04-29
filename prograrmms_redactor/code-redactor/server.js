const express = require('express');
const fs = require('fs').promises;
const path = require('path');
const app = express();
const port = 3000;

app.use(express.json());
app.use(express.static('.'));

// Получение структуры директории
async function getDirectoryTree(dirPath) {
    const items = await fs.readdir(dirPath, { withFileTypes: true });
    const tree = [];

    for (const item of items) {
        if (item.name.startsWith('.')) continue; // Пропускаем скрытые файлы

        const fullPath = path.join(dirPath, item.name);
        if (item.isDirectory()) {
            tree.push({
                name: item.name,
                path: fullPath,
                type: 'directory',
                children: await getDirectoryTree(fullPath)
            });
        } else {
            tree.push({
                name: item.name,
                path: fullPath,
                type: 'file'
            });
        }
    }

    return tree.sort((a, b) => {
        if (a.type === b.type) {
            return a.name.localeCompare(b.name);
        }
        return a.type === 'directory' ? -1 : 1;
    });
}

// API для получения дерева директории
app.get('/api/tree', async (req, res) => {
    try {
        const workingDir = req.query.path || '.';
        const tree = await getDirectoryTree(workingDir);
        res.json(tree);
    } catch (error) {
        res.status(500).json({ error: error.message });
    }
});

// API для чтения файла
app.get('/api/file', async (req, res) => {
    try {
        const filePath = req.query.path;
        if (!filePath) {
            throw new Error('Path parameter is required');
        }
        const content = await fs.readFile(filePath, 'utf-8');
        res.json({ content });
    } catch (error) {
        res.status(500).json({ error: error.message });
    }
});

// API для сохранения файла
app.post('/api/file', async (req, res) => {
    try {
        const { path: filePath, content } = req.body;
        if (!filePath) {
            throw new Error('Path parameter is required');
        }
        await fs.writeFile(filePath, content, 'utf-8');
        res.json({ success: true });
    } catch (error) {
        res.status(500).json({ error: error.message });
    }
});

app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
}); 