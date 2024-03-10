import Link from 'next/link';

import { SITE_CONFIG } from '../../config';

import { getAllPosts } from '@/lib/api';
import Footer from '@/app/_components/footer';
import ArticlePreview from '@/app/_components/article-preview';

export default function Index() {
    const allPosts = getAllPosts();

    const heroPost = allPosts[0];

    const morePosts = allPosts.slice(1);

    return (
        <main>
            <Link href="/" className="logo-readium">
                <span className="logo" style={{ backgroundImage: `url(${SITE_CONFIG.logo})` }}></span>
            </Link>
            <div className="page-content">
                <div className="wrapper">
                    <div className="teaserimage">
                        <div
                            className="teaserimage-image"
                            style={{ backgroundImage: `url(${SITE_CONFIG.coverImage})` }}
                        >
                            Teaser Image
                        </div>
                    </div>

                    <header className="blog-header">

                        <Link
                            className="blog-logo"
                            href="/"
                            style={{ backgroundImage: `url(${SITE_CONFIG.logo})` }}
                        >
                            {SITE_CONFIG.title}
                        </Link>

                        <h1 className="blog-title">{SITE_CONFIG.title}</h1>
                        <h2 className="blog-description">
                            {SITE_CONFIG.description}
                        </h2>
                        <div className="custom-links">
                            {SITE_CONFIG.customLinks.map((link, index) => (
                                <>
                                    <Link href={link.url} key={index} target="_blank">
                                        {/*@ts-ignore*/}
                                        {link?.icon ? (
                                        //   @ts-ignore
                                            <i className={link.icon}></i>
                                        ) : link.label}
                                    </Link>
                                    {index < SITE_CONFIG.customLinks.length - 1 && (
                                        <>&nbsp;&nbsp;·&nbsp;&nbsp;</>
                                    )}
                                </>
                            ))}
                        </div>
                    </header>


                    <main className="content" role="main">
                        <h5 className="index-headline featured"><span>Featured</span></h5>
                        <div className="container featured">
                            {heroPost && <ArticlePreview post={heroPost} />}
                        </div>

                        <h5 className="index-headline normal"><span>Regular</span></h5>
                        <div className="cf frame">
                            {morePosts.map((post, index) => (
                                <ArticlePreview post={post} key={index} />
                            ))}
                        </div>
                        {/*<nav className="pagination" role="navigation">*/}
                        {/*    <span className="page-number">Page 1 of 1</span>*/}
                        {/*</nav>*/}
                    </main>
                </div>
            </div>
            <Footer />
        </main>
    );
}
