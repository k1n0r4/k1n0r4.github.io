document.addEventListener("DOMContentLoaded", function() {
    window.addEventListener('scroll', function() {
        const image = document.getElementById('article-cover');
        if (image) {
            const scrollTop = window.scrollY || window.pageYOffset;
            const opacity = 1 - Math.max(scrollTop / 700, 0); // Adjust 700 according to your needs
            const translateY = scrollTop / 3; // Adjust 3 according to your needs
            image.style.opacity = opacity;
            image.style.transform = 'translateY(' + translateY + 'px)';
        }
    });
});
