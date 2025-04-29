function toggleFileExplorer() {
    const fileExplorer = document.getElementById('file-explorer');
    fileExplorer.classList.toggle('active');
}

// Mobile-specific functionality
document.addEventListener('DOMContentLoaded', function() {
    const mobileMenuToggle = document.getElementById('mobile-menu-toggle');
    const fileExplorer = document.getElementById('file-explorer');

    // Toggle file explorer on mobile menu button click
    mobileMenuToggle.addEventListener('click', toggleFileExplorer);

    // Close file explorer when clicking outside on mobile
    document.addEventListener('click', function(event) {
        const isClickInside = fileExplorer.contains(event.target) || 
                            mobileMenuToggle.contains(event.target);
        
        if (!isClickInside && fileExplorer.classList.contains('active') && 
            window.innerWidth <= 768) {
            fileExplorer.classList.remove('active');
        }
    });

    // Handle touch events for better mobile experience
    let touchStartX = 0;
    let touchEndX = 0;

    document.addEventListener('touchstart', function(event) {
        touchStartX = event.touches[0].clientX;
    }, false);

    document.addEventListener('touchend', function(event) {
        touchEndX = event.changedTouches[0].clientX;
        handleSwipe();
    }, false);

    function handleSwipe() {
        const swipeDistance = touchEndX - touchStartX;
        const threshold = 50; // minimum distance for swipe

        if (Math.abs(swipeDistance) >= threshold) {
            if (swipeDistance > 0 && touchStartX < 30) {
                // Swipe right from left edge - open file explorer
                fileExplorer.classList.add('active');
            } else if (swipeDistance < 0 && fileExplorer.classList.contains('active')) {
                // Swipe left - close file explorer
                fileExplorer.classList.remove('active');
            }
        }
    }

    // Adjust CodeMirror size on orientation change
    window.addEventListener('orientationchange', function() {
        setTimeout(function() {
            const editors = document.querySelectorAll('.CodeMirror');
            editors.forEach(editor => {
                editor.CodeMirror.refresh();
            });
        }, 100);
    });
}); 