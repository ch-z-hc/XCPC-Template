"use strict";

(() => {
  const entries = [...document.querySelectorAll(".entry")];
  entries.forEach(entry => { entry.hidden = true; });
  const links = [...document.querySelectorAll(".toc-links a")];
  const groups = [...document.querySelectorAll(".toc-group")];
  const byId = new Map(entries.map((entry, index) => [entry.id, {entry, index, link: links[index]}]));
  const search = document.querySelector("#search");
  const sidebar = document.querySelector(".sidebar");
  const menu = document.querySelector(".menu-button");
  const backdrop = document.querySelector(".backdrop");
  const reader = document.querySelector("#reader");
  const mobile = matchMedia("(max-width: 800px)");
  const status = document.querySelector("#copy-status");
  let current;
  let searchable;

  function setMenu(open, restoreFocus = true) {
    const wasOpen = document.body.classList.contains("toc-open");
    open = open && mobile.matches;
    document.body.classList.toggle("toc-open", open);
    backdrop.hidden = !open;
    menu.setAttribute("aria-expanded", String(open));
    menu.setAttribute("aria-label", open ? "关闭目录" : "打开目录");
    sidebar.inert = mobile.matches && !open;
    reader.inert = open;
    if (open) search.focus();
    else if (wasOpen && restoreFocus) menu.focus();
  }

  function filter() {
    const query = search.value.trim().toLowerCase();
    if (query && !searchable) {
      searchable = entries.map(entry =>
        `${entry.dataset.number} ${entry.dataset.title} ${entry.dataset.path} ${entry.querySelector("code").textContent}`.toLowerCase());
    }
    let count = 0;
    links.forEach((link, index) => {
      link.hidden = !!query && !searchable[index].includes(query);
      if (!link.hidden) count++;
    });
    groups.forEach(group => {
      group.hidden = ![...group.querySelectorAll("a")].some(link => !link.hidden);
      if (query) group.open = !group.hidden;
      else group.open = !!current && group.contains(current.link);
    });
    const result = document.querySelector(".search-status");
    result.hidden = !query;
    result.textContent = `找到 ${count} 个条目`;
    document.querySelector(".empty-search").hidden = count !== 0;
  }

  function route(focus = false) {
    if (location.hash === "#template" || location.hash === "#oeis") {
      location.replace("pdf.html" + location.hash);
      return;
    }
    let id;
    try { id = decodeURIComponent(location.hash.slice(1)); } catch { id = ""; }
    const next = byId.get(id) || byId.get(entries[0].id);
    if (current) {
      current.entry.hidden = true;
      current.link.removeAttribute("aria-current");
    }
    current = next;
    current.entry.hidden = false;
    current.link.setAttribute("aria-current", "page");
    current.link.closest("details").open = true;
    document.title = `${current.entry.dataset.title} · XCPC Template`;
    for (const [name, offset] of [["previous", -1], ["next", 1]]) {
      const anchor = document.getElementById(name);
      const entry = entries[current.index + offset];
      anchor.hidden = !entry;
      if (entry) {
        anchor.href = "#" + encodeURIComponent(entry.id);
        anchor.querySelector("span").textContent = `${entry.dataset.number} ${entry.dataset.title}`;
      }
    }
    status.textContent = "";
    setMenu(false, false);
    window.scrollTo(0, 0);
    if (focus) current.entry.querySelector("h1").focus({preventScroll: true});
  }

  function legacyCopy(text) {
    const active = document.activeElement;
    const field = document.createElement("textarea");
    field.value = text;
    field.readOnly = true;
    field.style.cssText = "position:fixed;top:0;left:-9999px;opacity:0";
    document.body.append(field);
    field.select();
    try {
      if (!document.execCommand("copy")) throw new Error("copy failed");
    } finally {
      field.remove();
      active?.focus({preventScroll: true});
    }
  }

  document.addEventListener("click", async event => {
    const button = event.target.closest(".copy-button");
    if (!button) return;
    const entry = button.closest(".entry");
    const code = entry.querySelector("code");
    button.disabled = true;
    try {
      try {
        if (!navigator.clipboard?.writeText) throw new Error("clipboard unavailable");
        await navigator.clipboard.writeText(code.textContent);
      } catch {
        legacyCopy(code.textContent);
      }
      button.classList.add("copied");
      button.querySelector("span").textContent = "已复制";
      status.textContent = `已复制：${entry.dataset.title}`;
    } catch {
      const range = document.createRange();
      range.selectNodeContents(code);
      getSelection().removeAllRanges();
      getSelection().addRange(range);
      button.querySelector("span").textContent = "请手动复制";
      status.textContent = "自动复制失败，代码已选中，请手动复制。";
    } finally {
      setTimeout(() => {
        button.classList.remove("copied");
        button.querySelector("span").textContent = "复制";
        button.disabled = false;
      }, 1800);
    }
  });

  search.addEventListener("input", filter);
  search.addEventListener("keydown", event => {
    if (event.key === "Enter") {
      const first = links.find(link => !link.hidden);
      if (first) first.click();
    }
  });
  links.forEach(link => link.addEventListener("click", () => {
    if (link === current?.link) {
      setMenu(false, false);
      current.entry.querySelector("h1").focus({preventScroll: true});
      window.scrollTo(0, 0);
    }
  }));
  menu.addEventListener("click", () => setMenu(menu.getAttribute("aria-expanded") !== "true"));
  document.querySelector(".skip-link").addEventListener("click", event => {
    event.preventDefault();
    setMenu(false, false);
    reader.focus();
  });
  backdrop.addEventListener("click", () => setMenu(false));
  mobile.addEventListener("change", () => setMenu(false));
  document.addEventListener("keydown", event => {
    const editing = event.target.matches("input,textarea,[contenteditable=true]");
    if ((event.key === "/" && !editing && !event.ctrlKey && !event.metaKey && !event.altKey) ||
        (event.key.toLowerCase() === "k" && (event.ctrlKey || event.metaKey))) {
      event.preventDefault();
      if (mobile.matches) setMenu(true);
      search.focus();
    }
    if (event.key === "Escape") {
      if (search.value && document.activeElement === search) {
        search.value = "";
        filter();
      } else setMenu(false);
    }
    if (event.key === "Tab" && document.body.classList.contains("toc-open")) {
      const items = [...sidebar.querySelectorAll("input, summary, a[href]")]
        .filter(item => item.getClientRects().length);
      const first = items[0], last = items.at(-1);
      if (event.shiftKey && document.activeElement === first) {
        event.preventDefault(); last.focus();
      } else if (!event.shiftKey && document.activeElement === last) {
        event.preventDefault(); first.focus();
      }
    }
  });
  addEventListener("hashchange", () => route(true));
  route();
})();
