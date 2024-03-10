import fs from 'fs';
import { join } from 'path';

import matter from 'gray-matter';

import { Post } from '@/interfaces/post';

const postsDirectory = join(process.cwd(), '_posts');

export function getPostSlugs() {
    return fs.readdirSync(postsDirectory);
}

function extractExcerpt(content: string) {
    const paragraphs = content.split(/\r?\n\r?\n/);
    for(const paragraph of paragraphs) {
        const plainText = paragraph.replace(/[#*>`]/g, '').trim();
        if(plainText.length > 100 && !/[^\]]\(/.test(plainText)) return plainText;
    }
    return null;
}

export function getPostBySlug(slug: string) {
    const realSlug = slug.replace(/\.md$/, '');
    const fullPath = join(postsDirectory, `${realSlug}.md`);
    const fileContents = fs.readFileSync(fullPath, 'utf8');
    const { data, content } = matter(fileContents);
    const excerpt = extractExcerpt(content);
    return { ...data, slug: realSlug, excerpt, content } as Post;
}

export function getAllPosts(): Post[] {
    const slugs = getPostSlugs();
    const posts = slugs
        .filter((slug) => slug !== 'about')
        .map((slug) => getPostBySlug(slug))
        .sort((post1, post2) => (post1.date > post2.date ? -1 : 1));
    return posts;
}
