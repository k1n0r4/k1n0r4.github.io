// import fs from "fs";
// import {join} from "path";
import { notFound } from 'next/navigation';

const AboutPage = () => {

    // const postsDirectory = join(process.cwd(), "_posts");
    // const fileContents = fs.readFileSync(join(postsDirectory, 'about.md'), "utf8");
    return notFound();

    // return (
    //     <div>
    //         <h1>About</h1>
    //         <div dangerouslySetInnerHTML={{ __html: fileContents }} />
    //     </div>
    // );
};

export default AboutPage;