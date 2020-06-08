import * as path from "https://deno.land/std/path/mod.ts";

interface EnumDeclaration {
    filename: string,
    name: string,
    value_type: string,
    values: Object,
    entries: [string, any][]
}

class EnumBuilder {
    enumDecl: EnumDeclaration | null
    code: string[]
    filepath: string
    dirname: string
    headerFilename: string
    headerFilepath: string
    metaHeader: string

    constructor() {
        this.enumDecl = null
        this.code = []
        this.filepath = ''
        this.dirname = ''
        this.headerFilename = ''
        this.headerFilepath = ''
        this.metaHeader = ''
    }

    setMetaHeader(filepath: string) {
        this.metaHeader = filepath
    }

    load(filepath: string): boolean {
        try {
            let f = new Function(`
                ${new TextDecoder().decode(Deno.readFileSync(filepath))}
                return _enum()
            `)
    
            this.enumDecl = f(filepath)
            
            if(!this.enumDecl) {
                return false
            }

            this.enumDecl.entries = Object.entries(this.enumDecl.values)
            this.filepath = filepath
            this.dirname = path.dirname(this.filepath)
            this.headerFilename = path.basename(this.filepath).replace('.ts', '.h')
            this.headerFilepath = path.join(this.dirname, this.headerFilename)

            return true
        } catch(err) {
            throw err
        }
    }

    generateCode(): boolean {
        if(!this.enumDecl || !this.metaHeader) {
            return false
        }

        let define = this.headerFilename.replace(/\./g, '_').toUpperCase()
        this.code.push(`#ifndef ${define}`)
        this.code.push(`#define ${define}`)
        this.code.push(`#include "${this.metaHeader}"`)

        this.generateEnumDecl(this.enumDecl)
        this.generateEnumNameTemplate(this.enumDecl)
        this.generateEnumEntriesTemplate(this.enumDecl)

        this.code.push(`#endif`)
        
        return true
    }

    getSource(): string {
        return this.code.join('\n')
    }

    private generateEnumDecl(enumDecl: EnumDeclaration) {
        this.code.push(`enum class ${enumDecl.name} {`)
        this.code = this.code.concat(enumDecl.entries.map(value => `    ${value[0]} = ${value[1]},`))
        this.code.push('};')
    }

    private generateEnumEntriesTemplate(enumDecl: EnumDeclaration) {
        this.code.push(`template<> struct enum_entries<${enumDecl.name}> {`)
        this.code.push(`    using value_type = std::array<enum_entry<${enumDecl.name}>, ${enumDecl.entries.length}>;`)
        this.code.push(`    constexpr static value_type value = {`)
        this.code = this.code.concat(enumDecl.entries.map(entry => `        enum_entry<${enumDecl.name}>{"${entry[0]}", ${enumDecl.name}::${entry[0]}},`))
        this.code.push(`    };`)
        this.code.push('};')
    }

    private generateEnumNameTemplate(enumDecl: EnumDeclaration) {
        for(let i = 0; i < enumDecl.entries.length; i++) {
            this.code.push(`template<> struct enum_name<${enumDecl.name}, ${enumDecl.name}::${enumDecl.entries[i][0]}> {`)
            this.code.push('    using value_type = const char *;')
            this.code.push(`    constexpr static value_type value = "${enumDecl.entries[i][0]}";`)
            this.code.push('};')
        }
    }

    /*
    private generateEnumSizeTemplate(enumDecl: EnumDeclaration) {
        this.code.push(`template<> struct enum_size<${enumDecl.name}> {`)
        this.code.push('    using value_type = std::size_t;')
        this.code.push(`    constexpr static value_type value = ${enumDecl.entries.length};`)
        this.code.push('};')
    }

    private generateEnumNameTemplate(enumDecl: EnumDeclaration) {
        for(let i = 0; i < enumDecl.entries.length; i++) {
            this.code.push(`template<> struct enum_name<${enumDecl.name}, ${i}> {`)
            this.code.push('    using value_type = const char *;')
            this.code.push(`    constexpr static value_type value = "${enumDecl.entries[i][0]}";`)
            this.code.push('};')
        }
    }

    
    private generateEnumValueTemplate(enumDecl: EnumDeclaration) {
        for(let i = 0; i < enumDecl.entries.length; i++) {
            this.code.push(`template<> struct enum_value<${enumDecl.name}, ${i}> {`)
            this.code.push(`    using value_type = ${enumDecl.value_type};`)
            this.code.push(`    constexpr static value_type value = ${enumDecl.entries[i][1]};`)
            this.code.push('};')
        }
    }
    */
}

let enumBuilder = new EnumBuilder()
enumBuilder.load('./src/Color.enum.ts')
enumBuilder.setMetaHeader('enum_meta.h')
enumBuilder.generateCode()

const encoder = new TextEncoder()
Deno.writeFileSync(enumBuilder.headerFilepath, encoder.encode(enumBuilder.getSource()))
