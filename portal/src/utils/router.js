const pages = {};
let currentPage = null;
let configData = null;

export function registerPage(id, pageModule) {
  pages[id] = pageModule;
}

export function setConfigData(data) {
  configData = data;
  const page = pages[currentPage];
  if (page && page.setData) page.setData(configData);
}

export function navigate(pageId) {
  const page = pages[pageId];
  if (!page) return;
  currentPage = pageId;

  document.querySelectorAll('.panel').forEach(p => p.classList.remove('active'));
  const panel = document.getElementById('panel-' + pageId);
  if (panel) panel.classList.add('active');

  const actionBar = document.getElementById('action-bar');
  if (page.showActions === false) {
    actionBar.style.display = 'none';
  } else {
    actionBar.style.display = '';
  }

  if (page.init) page.init();
  if (configData && page.setData) page.setData(configData);

  document.querySelectorAll('.nav-item').forEach(item => {
    item.classList.toggle('active', item.dataset.panel === pageId);
  });

  if (window.innerWidth <= 768) {
    document.getElementById('sidebar').classList.remove('open');
  }
}

export function getCurrentPageId() { return currentPage; }
export function getPage(id) { return pages[id]; }
export function getAllPages() { return pages; }
