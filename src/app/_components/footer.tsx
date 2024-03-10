import { SITE_CONFIG } from '../../../config';

export function Footer() {
    return (
        <footer className="site-footer">
            <div className="inner">
                <section className="copyright">
                    All content copyright &nbsp;
                    <a href={'mailto:' + SITE_CONFIG.author.mail}>
                        {SITE_CONFIG.author.name}
                    </a>
                    {' '}
                    &copy;
                    {new Date().getFullYear()}
                    {' '}
                    &bull; All rights reserved.
                </section>
            </div>
        </footer>
    );
}

export default Footer;
