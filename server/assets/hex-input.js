(() => {
  function sanitizeHexInput(value) {
    const normalized = value
      .toUpperCase()
      .replace(/[^0-9A-F\s,;:-]/g, "")
      .replace(/[\s,;:-]+/g, "");

    return normalized.match(/.{1,2}/g)?.join(" ") ?? "";
  }

  function hasHexInput(value) {
    return value.trim().length > 0;
  }

  window.hexInputUtils = {
    sanitizeHexInput,
    hasHexInput,
  };
})();
